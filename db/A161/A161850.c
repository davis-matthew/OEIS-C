/*
 * A161850  Subsequence of A161986 consisting of all terms that are prime.
 * https://oeis.org/A161850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime value k + r where k is the nth composite number and
 *            r = (largest prime divisor of k) modulo (smallest prime divisor
 *            of k)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th term of OEIS A161850 (prime subsequence of A161986). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Miller–Rabin helpers (64‑bit only) ---------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return ((unsigned __int128)a * b) % mod;
}

static inline uint64_t pow_mod_u64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = mul_mod_u64(res, a, mod);
        a = mul_mod_u64(a, a, mod);
        d >>= 1;
    }
    return res;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    if (n % 5ULL == 0ULL) return n == 5ULL;

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                              450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- integer square root ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        uint64_t sq = (unsigned __int128)mid * mid;
        if (sq <= n) { ans = mid; low = mid + 1; }
        else high = mid - 1;
    }
    return ans;
}

/* ---------- prime sieve for factorisation ---------- */
static uint64_t *prime_list = NULL;
static size_t       prime_cnt = 0;
static size_t       prime_cap = 0;
static uint64_t     last_checked = 1;

/* grow the prime list up to at least 'limit' */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_checked) return;

    for (uint64_t cand = last_checked + 1; cand <= limit; ++cand) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16U : prime_cap << 1;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;
        }
    }
    last_checked = limit;
}

/* initialise prime list with first prime 2 */
static void init_prime_list(void)
{
    if (prime_list) return;
    prime_cap = 16U;
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    prime_list[prime_cnt++] = 2ULL;
    last_checked = 2ULL;
}

/* smallest and largest prime factors of a composite k */
static void get_spf_lpf(uint64_t k, uint64_t *spf, uint64_t *lpf)
{
    uint64_t temp = k;
    *spf = 0; *lpf = 0;

    uint64_t limit = isqrt_u64(k);
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (temp % p == 0) {
            if (*spf == 0) *spf = p;
            *lpf = p;
            while (temp % p == 0) temp /= p;
            limit = isqrt_u64(temp);
        }
    }
    if (temp > 1) {                /* remaining factor is prime > sqrt(original) */
        if (*spf == 0) *spf = temp;
        *lpf = temp;
    }
}

/* ---------- memoised sequence storage ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* ---------- main entry ---------- */
seq_t a(idx_t n)
{
    init_prime_list();
    if (!seq_cache) { seq_cap = 16; seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t)); }

    static uint64_t next_composite = 4;   /* first composite number */

    /* generate until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find next composite (skip primes) */
        while (is_prime_u64(next_composite)) ++next_composite;
        uint64_t k = next_composite;

        /* factors of k */
        uint64_t spf = 0, lpf = 0;
        get_spf_lpf(k, &spf, &lpf);

        /* A161986 value */
        uint64_t r   = lpf % spf;
        uint64_t val = k + r;

        /* store if prime */
        if (is_prime_u64(val)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)val;
        }

        ++next_composite;   /* move to next composite for next iteration */
    }

    return seq_cache[n - 1];
}
