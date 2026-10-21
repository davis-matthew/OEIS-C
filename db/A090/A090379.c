/*
 * A090379  a(n) = abs(floor((pi(n)-2)/(2-pi(n)+pi(n-1)))), where pi(n) is
 *          A000720(n).
 * https://oeis.org/A090379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = abs(floor((pi(n)-2)/(2-pi(n)+pi(n-1))))
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   floor_div64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_div64.h"

/* Dynamic sieve state */
static uint8_t *is_prime   = NULL;      /* 1 if prime, 0 otherwise */
static uint32_t *pi_prefix = NULL;      /* cumulative prime counts */
static uint64_t  sieve_limit = 0;       /* highest index currently sieved */

/* Integer square‑root by binary search (no <math.h> needed). */
static inline uint64_t int_sqrt(uint64_t x)
{
    uint64_t lo = 0, hi = x, mid = 0, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) { res = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return res;
}

/* Extend the sieve so that pi(n) is available for all n ≤ need */
static void extend_sieve(uint64_t need)
{
    if (need <= sieve_limit) return;

    /* Grow the limit to a power of two (reduces reallocs) */
    uint64_t new_limit = sieve_limit;
    if (new_limit == 0) new_limit = 2;
    while (new_limit < need) new_limit <<= 1;

    /* Allocate or enlarge the arrays */
    is_prime = (uint8_t*)realloc(is_prime, (new_limit + 1) * sizeof(uint8_t));
    pi_prefix = (uint32_t*)realloc(pi_prefix, (new_limit + 1) * sizeof(uint32_t));
    if (!is_prime || !pi_prefix) { exit(1); }

    /* Initialise the new segment as “prime” */
    memset(is_prime + sieve_limit + 1, 1, (new_limit - sieve_limit) * sizeof(uint8_t));
    is_prime[0] = 0;
    is_prime[1] = 0;

    uint64_t start = sieve_limit + 1;
    uint64_t sqrt_new = int_sqrt(new_limit);

    /* Sieve composites in the new range */
    for (uint64_t p = 2; p <= sqrt_new; ++p) {
        if (!is_prime[p]) continue;
        uint64_t first = (start + p - 1) / p;
        first *= p;
        if (first < p * p) first = p * p;
        for (uint64_t j = first; j <= new_limit; j += p)
            is_prime[j] = 0;
    }

    /* Build cumulative prime counts for the new segment */
    if (sieve_limit == 0) pi_prefix[0] = pi_prefix[1] = 0;
    for (uint64_t i = start; i <= new_limit; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + is_prime[i];
    }

    sieve_limit = new_limit;
}

/* Sequence term a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined region */

    extend_sieve((uint64_t)n);

    uint32_t pi_n = pi_prefix[n];
    /* pi(n-1) is not needed explicitly because the denominator is
       1 when n is prime and 2 otherwise. */
    long long numerator = (long long)pi_n - 2;
    long long value;

    if (is_prime[n]) {
        /* denominator == 1 */
        value = numerator;
    } else {
        /* denominator == 2, need floor division for negatives */
        value = floor_div64(numerator, 2);
    }

    if (value < 0) value = -value;       /* absolute value */

    return (seq_t)value;
}
