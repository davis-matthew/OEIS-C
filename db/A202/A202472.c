/*
 * A202472  Goldbach's Problem extended to subtraction: number of
 *          decompositions of 2n into unordered differences of two primes, p,
 *          q, where p < 2n < q.
 * https://oeis.org/A202472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{prime p : p < 2n and p + 2n is prime}
 * Range:     u8 n <= 1784, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global dynamic sieve storage */
static unsigned char *is_prime = NULL; /* 0 = non‑prime, 1 = prime */
static idx_t max_sieved   = 0;        /* largest index already sieved */

/* Integer square root for idx_t (binary search). */
static idx_t _int_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if ((mid * mid) <= n) {
            ans = mid;
            lo  = mid + 1;
        } else
            hi = mid - 1;
    }
    return ans;
}

/* Extend the sieve so that indices up to 'limit' are classified
   correctly as prime or composite. */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= max_sieved) return;

    /* Grow the storage if necessary. */
    size_t new_cap = (size_t)(limit + 1);
    unsigned char *new_tbl = (unsigned char*)realloc(is_prime, new_cap);
    if (!new_tbl) abort();          /* allocation failed */
    is_prime = new_tbl;

    /* Initialise new entries as prime (until sieved). */
    for (size_t i = max_sieved + 1; i <= (size_t)limit; ++i)
        is_prime[i] = 1;

    /* 0 and 1 are not prime. */
    if (max_sieved < 1) {
        if (new_cap > 0) is_prime[0] = 0;
        if (new_cap > 1) is_prime[1] = 0;
    }

    /* Perform a full sieving sweep up to 'limit'. */
    idx_t sqrt_lim = _int_isqrt_u64(limit);
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (!is_prime[p]) continue;

        /* First multiple of p that is >= max_sieved+1. */
        idx_t start = (max_sieved + 1 + p - 1) / p * p;
        if (start < p * p) start = p * p;

        for (idx_t j = start; j <= limit; j += p)
            is_prime[j] = 0;
    }

    max_sieved = limit;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */

    idx_t two_n = n + n;
    idx_t limit = two_n + two_n;    /* 4n is sufficient for p+2n */

    _ensure_sieve(limit);

    seq_t count = 0;

    /* Count primes p < 2n with p+2n also prime. */
    for (idx_t p = 2; p < two_n; ++p) {
        if (is_prime[p] && is_prime[p + two_n])
            ++count;
    }
    return count;
}
