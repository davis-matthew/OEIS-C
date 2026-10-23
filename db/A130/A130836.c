/*
 * A130836  Square array d(m,n) = multiplicative distance between m>=1 and
 *          n>=1, read by antidiagonals.
 * https://oeis.org/A130836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9981, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   bigomega, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative distance d(m,n) for the index n by
   mapping n to the antidiagonal pair (m,n) and summing the absolute
   differences of exponents in their prime factorizations. */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/bigomega.h"

static inline idx_t int_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the diagonal k such that (k-1)k/2 >= n */
    idx_t low = 2, high = 2;
    while ((((__uint128_t)high * (high - 1)) / 2) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((((__uint128_t)mid * (mid - 1)) / 2) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t k = low;  /* diagonal index (k = m + n) */

    /* Offset within the diagonal */
    idx_t prev_total = (idx_t)((( (__uint128_t)(k - 2) * (k - 1)) / 2));
    idx_t m = n - prev_total;
    idx_t n_val = k - m;   /* second coordinate */

    /* Compute prime factor sums */
    seq_t res = bigomega(m) + bigomega(n_val)
        - (seq_t)2 * bigomega(int_gcd(m, n_val));
    return res;
}
