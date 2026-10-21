/*
 * A075019  a(1) = 1; for n > 1, a(n) = the smallest prime divisor of the
 *          number C(n) formed from the concatenation of 1,2,3,... up to n.
 * https://oeis.org/A075019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A075019: the smallest prime divisor of 
 * C(n) = 123…n (concatenation of 1,2,…,n).
 * For n = 1 we return 1; for even n the result is 2.
 * For odd n we find the least prime that divides C(n).
 * The algorithm tests primes incrementally; if no prime ≤
 * PRIME_LIMIT divides C(n) we continue testing odd integers ≥
 * PRIME_LIMIT+1.  It uses 64‑bit arithmetic for all intermediate
 * calculations to avoid overflow.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t PRIME_LIMIT = 200000;   /* sieve bound – generous enough for n ≤ 10 000 */

/* -------- prime sieve ---------------------------------------------- */
static bool primes_built = false;
static size_t prime_cnt   = 0;
static idx_t *primes      = NULL;          /* primes[i] holds the i‑th prime (i≥0).   */

/* Build a list of primes up to PRIME_LIMIT.  */
static void build_primes(void)
{
    size_t sz = PRIME_LIMIT + 1;
    bool *is_comp = (bool*)calloc(sz, sizeof(bool));
    if (!is_comp) abort();

    for (idx_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (!is_comp[p])
            for (idx_t mult = p * p; mult <= PRIME_LIMIT; mult += p)
                is_comp[mult] = true;

    /* Count primes first */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_cnt++;

    primes = (idx_t*)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    free(is_comp);
    primes_built = true;
}

/* -------- helpers ---------------------------------------------- */

/* Number of decimal digits in x (x ≥ 0). */
static inline int digit_len(idx_t x)
{
    if (x >= 1000000000000000000ULL) return 19;
    if (x >= 100000000000000000ULL)  return 18;
    if (x >= 10000000000000000ULL)   return 17;
    if (x >= 1000000000000000ULL)    return 16;
    if (x >= 100000000000000ULL)     return 15;
    if (x >= 10000000000000ULL)      return 14;
    if (x >= 1000000000000ULL)       return 13;
    if (x >= 100000000000ULL)        return 12;
    if (x >= 10000000000ULL)         return 11;
    if (x >= 1000000000ULL)          return 10;
    if (x >= 100000000ULL)           return 9;
    if (x >= 10000000ULL)            return 8;
    if (x >= 1000000ULL)             return 7;
    if (x >= 100000ULL)              return 6;
    if (x >= 10000ULL)               return 5;
    if (x >= 1000ULL)                return 4;
    if (x >= 100ULL)                 return 3;
    if (x >= 10ULL)                  return 2;
    return 1;
}

/* -------- main sequence function ---------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety, not used by OEIS */
    if (n == 1) return 1;
    if ((n & 1u) == 0) return (seq_t)2;   /* even n ⇒ C(n) even ⇒ factor 2 */

    if (!primes_built) build_primes();

    /* Helper array for powers of ten modulo a candidate divisor.
     * 10^d mod cand   for d = 0 … max_digits.  Array length fixed at 21
     * as n fits in 64‑bit → at most 20 digits. */
    static idx_t pow10[21]; /* pow10[d] = 10^d mod cand */

    /* Search primes ≤ PRIME_LIMIT first. */
    for (size_t pi = 0; pi < prime_cnt; ++pi)
    {
        idx_t cand = primes[pi];

        /* Precompute powers of ten modulo cand. */
        pow10[0] = 1 % cand;
        int maxd = digit_len(n);
        for (int d = 1; d <= maxd; ++d)
            pow10[d] = (pow10[d-1] * 10) % cand;

        /* Compute C(n) mod cand. */
        idx_t rem = 0;
        int d = 1;
        idx_t next_ten = 10;
        for (idx_t k = 1; k <= n; ++k)
        {
            if (k == next_ten) { d++; next_ten *= 10; }
            rem = (rem * pow10[d] + k) % cand;
        }
        if (rem == 0) return (seq_t)cand;
    }

    /* No small prime divides C(n): continue with odd integers ≥ PRIME_LIMIT+1. */
    idx_t cand = primes[prime_cnt-1] + 2;   /* start from first odd after max prime */
    for (;; cand += 2)
    {
        /* Precompute powers of ten modulo cand. */
        pow10[0] = 1 % cand;
        int maxd = digit_len(n);
        for (int d = 1; d <= maxd; ++d)
            pow10[d] = (pow10[d-1] * 10) % cand;

        /* Compute C(n) mod cand. */
        idx_t rem = 0;
        int d = 1;
        idx_t next_ten = 10;
        for (idx_t k = 1; k <= n; ++k)
        {
            if (k == next_ten) { d++; next_ten *= 10; }
            rem = (rem * pow10[d] + k) % cand;
        }
        if (rem == 0) return (seq_t)cand;
    }
}
