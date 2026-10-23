/*
 * A043292  Numbers that divide 10^k + 1 for some k.
 * https://oeis.org/A043292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer d such that ∃k≥0 with d | 10^k +
 *            1
 * Range:     u8 n <= 47, u16 n <= 6516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For d>2 this is equivalent to: d odd, gcd(d,10)=1 and ∃k with 10^k ≡ -1 (mod d) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"
#include "../utils/ensure_cache.h"

/* ---------- Utility: GCD for uint64_t ---------- */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline uint64_t lcm_u64(uint64_t a, uint64_t b)
{
    if (a == 0 || b == 0) return a | b;
    uint64_t g = gcd_u64(a, b);
    __uint128_t prod = (__uint128_t)(a / g) * b;
    if (prod > (uint64_t)-1) return (uint64_t)-1;   /* overflow sentinel */
    return (uint64_t)prod;
}

/* ---------- Sieve of Eratosthenes ---------- */
#define SIEVE_MAX 5000000
static bool sieve_composite[SIEVE_MAX + 1] = {0};
static uint32_t primes_list[400000];          /* ~348,513 primes up to 5 000 000 */
static size_t primes_count = 0;
static bool primes_generated = false;

static void gen_primes(void)
{
    if (primes_generated) return;
    for (uint32_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!sieve_composite[i]) {
            primes_list[primes_count++] = i;
            if ((uint64_t)i * i <= SIEVE_MAX) {
                for (uint32_t j = i * i; j <= SIEVE_MAX; j += i)
                    sieve_composite[j] = 1;
            }
        }
    }
    primes_generated = true;
}

/* ---------- Factorisation of a 64‑bit integer ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t fac[20], uint8_t exps[20], size_t *cnt)
{
    size_t idx = 0;
    for (size_t i = 0; i < primes_count && (uint64_t)primes_list[i] *
         (uint64_t)primes_list[i] <= n; ++i) {
        uint64_t p = primes_list[i];
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fac[idx] = p;
            exps[idx] = e;
            ++idx;
        }
    }
    if (n > 1) {
        fac[idx] = n;
        exps[idx] = 1;
        ++idx;
    }
    *cnt = idx;
}

/* ---------- Modular exponentiation for 64‑bit modulus ---------- */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod64(result, b, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- Test whether d satisfies the sequence condition ---------- */
static bool ok_for_d(uint64_t d)
{
    if (d == 1 || d == 2) return true;                     /* trivial cases */
    if ((d & 1ULL) == 0ULL || d % 5ULL == 0ULL) return false; /* even or multiple of 5 */

    uint64_t fac[20];
    uint8_t exps[20];
    size_t fac_cnt;
    factor_uint64(d, fac, exps, &fac_cnt);

    uint64_t lcm_half = 1;

    for (size_t idx = 0; idx < fac_cnt; ++idx) {
        uint64_t p = fac[idx];
        uint8_t e   = exps[idx];

        /* compute p^e */
        uint64_t p_pow = 1;
        for (uint8_t i = 0; i < e; ++i) p_pow *= p;

        /* φ(p^e) = p^e – p^(e-1) */
        uint64_t phi = p_pow - p_pow / p;
        uint64_t ord = phi;                    /* start with φ */

        /* factor φ to reduce the order */
        uint64_t ord_fac[20];
        uint8_t  ord_ex[20];
        size_t   ord_cnt;
        factor_uint64(phi, ord_fac, ord_ex, &ord_cnt);

        for (size_t j = 0; j < ord_cnt; ++j) {
            uint64_t q = ord_fac[j];
            for (uint8_t k = 0; k < ord_ex[j]; ++k) {
                if (ord % q != 0) break;
                if (powmod64(10ULL, ord / q, p_pow) == 1ULL)
                    ord /= q;
                else
                    break;
            }
        }

        if (ord & 1ULL) return false;        /* odd order → impossible to get -1 */

        uint64_t half = ord / 2;
        uint64_t new_lcm = lcm_u64(lcm_half, half);
        if (new_lcm == (uint64_t)-1) return false; /* overflow → reject */
        lcm_half = new_lcm;
    }

    /* Verify that 10^lcm_half ≡ -1 (mod d) */
    return powmod64(10ULL, lcm_half, d) == d - 1;
}

/* ---------- Storage of computed sequence ---------- */
static seq_t *seq_cache      = NULL;
static size_t cache_len      = 0;   /* number of terms stored */
static size_t cache_cap      = 0;   /* capacity of seq_cache   */
static uint64_t next_candidate = 1; /* next integer to test */

/* Ensure that at least n terms are stored in seq_cache. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;
        ensure_cache((idx_t)new_cap, &seq_cache, &cache_cap);
    }

    gen_primes();                 /* initialise prime list */

    while (cache_len < (size_t)n) {
        if (ok_for_d(next_candidate))
            seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq_cache[n - 1];
}
