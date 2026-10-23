/*
 * A140292  a(n) is a square mod a(n-1), a(n) > a(n-1) and a(n) semiprime.
 * https://oeis.org/A140292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140292: a(1)=4; for n>1, a(n) is the smallest semiprime > a(n‑1)
   that is a quadratic residue modulo a(n‑1).  The sequence starts 4,9,10,
   14,15,21,22,25,26,35,39,49,51,55,69,82,86,87,91,95,106,115,119,121. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Prime generation up to a fixed limit --------------------- */
static uint32_t *prime_list = NULL;
static size_t   prime_count = 0;

static void init_primes(void)
{
    if (prime_list) return;                /* already done */
    const unsigned limit = 200000;         /* enough for all searches */
    bool *sieve = (bool *)malloc(limit + 1);
    memset(sieve, 0, limit + 1);
    for (unsigned i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (unsigned j = i * i; j <= limit; j += i)
                sieve[j] = 1;
    for (unsigned i = 2; i <= limit; ++i)
        if (!sieve[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t k = 0;
    for (unsigned i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[k++] = i;
    free(sieve);
}

/* ---------- modular exponentiation over uint64_t ---------------------- */
static uint64_t powmod_uint(uint64_t a, uint64_t e, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t r = 1;
    uint64_t b = a % mod;
    while (e) {
        if (e & 1ULL) r = (unsigned __int128)r * b % mod;
        b = (unsigned __int128)b * b % mod;
        e >>= 1ULL;
    }
    return r;
}

/* ---------- integer power (small exponents) --------------------------- */
static uint64_t pow_uint(uint64_t base, unsigned exp)
{
    uint64_t r = 1;
    while (exp--) r *= base;
    return r;
}

/* ---------- factorisation of uint64_t using precomputed primes -------- */
static void factor_uint64(uint64_t n, uint32_t primes[], size_t primes_cnt,
                          uint32_t out_primes[], uint32_t out_exps[],
                          size_t *out_cnt)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt &&
         ((uint64_t)primes[i] * primes[i] <= tmp); ++i)
    {
        if (tmp % primes[i] == 0) {
            uint32_t p = primes[i];
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            out_primes[cnt] = p;
            out_exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                     /* remaining factor > sqrt(tmp)   */
        out_primes[cnt] = (uint32_t)tmp;
        out_exps[cnt]   = 1;
        ++cnt;
    }
    *out_cnt = cnt;
}

/* ---------- check for semiprime ------------------------------------- */
static bool is_semiprime_uint(uint64_t x)
{
    if (x < 4) return false;
    size_t factor_cnt = 0;
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_count &&
         ((uint64_t)prime_list[i] * prime_list[i] <= tmp); ++i) {
        if (tmp % prime_list[i] == 0) {
            int e = 0;
            while (tmp % prime_list[i] == 0 && factor_cnt < 3) {
                tmp /= prime_list[i];
                ++e;
                ++factor_cnt;
                if (factor_cnt > 2) return false;
            }
        }
    }
    if (tmp > 1) {                     /* remaining prime factor         */
        ++factor_cnt;
        if (factor_cnt > 2) return false;
    }
    return factor_cnt == 2;
}

/* ---------- quadratic residue test modulo m ------------------------ */
static bool is_quadratic_residue_mod(uint64_t r, uint64_t m)
{
    if (m == 1) return true;
    uint32_t fac_primes[16];
    uint32_t fac_exps[16];
    size_t fac_cnt;
    factor_uint64(m, prime_list, prime_count,
                  fac_primes, fac_exps, &fac_cnt);

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = fac_primes[i];
        uint32_t e = fac_exps[i];
        uint64_t modpow = pow_uint((uint64_t)p, e);
        uint64_t r_mod = r % modpow;

        if (p == 2) {
            if (e == 1) continue;                         /* mod 2 */
            if (e == 2) {                                 /* mod 4 */
                uint64_t r4 = r_mod % 4;
                if (r4 != 0 && r4 != 1) return false;
            } else {                                      /* mod 8+ */
                uint64_t r8 = r_mod % 8;
                if (r8 != 0 && r8 != 1 && r8 != 4) return false;
            }
        } else {                                          /* odd prime */
            uint64_t base = r_mod % p;
            if (base == 0) continue;                     /* divisible */
            uint64_t leg = powmod_uint(base, (p - 1) >> 1, p);
            if (leg != 1) return false;
        }
    }
    return true;
}

/* ---------- cache for previously computed terms --------------------- */
static seq_t *cache  = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* ---------- primary function ---------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_primes(); initialized = true; }
    if (n == 0) return 0;   /* undefined */

    /* grow cache up to required index */
    while (cache_len < (size_t)n) {
        seq_t new_term;
        if (cache_len == 0) {           /* a(1) */
            new_term = 4;
        } else {
            uint64_t prev = (uint64_t)cache[cache_len - 1];
            uint64_t cand = prev + 1;
            for (;;) {
                if (is_semiprime_uint(cand)) {
                    uint64_t r = cand % prev;
                    if (is_quadratic_residue_mod(r, prev)) {
                        new_term = (seq_t)cand;
                        break;
                    }
                }
                ++cand;
            }
        }

        /* ensure capacity */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = new_term;
        ++cache_len;
    }
    return cache[n - 1];
}
