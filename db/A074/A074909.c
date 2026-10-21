/*
 * A074909  Running sum of Pascal's triangle (A007318), or beheaded Pascal's
 *          triangle read by beheaded rows.
 * https://oeis.org/A074909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r+1, k) where r = floor( (sqrt(8*n+1)-1)/2 ) and
 *            k = n - r*(r+1)/2. This counts the beheaded rows of Pascal's
 *            triangle
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 611, u64 n <= 2308,
 *            u128 n <= 8709, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the pre‑provided gcd routine. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) safely using 128‑bit
   intermediate multiplication and gcd reduction to avoid overflow
   when seq_t is a 64‑bit integer.  Assumes n,k fit in idx_t. */
static seq_t binom_custom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k;                /* use symmetry */

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator = (seq_t)(n - k + i);  /* (n-k+i) */
        seq_t denom     = (seq_t)i;

        seq_t g = gcd(result, denom);
        result /= g;
        denom   /= g;

        g = gcd(numerator, denom);
        numerator /= g;
        denom     /= g;

        /* denom should be 1 now; use 128‑bit to avoid overflow. */
        result = (seq_t)(((unsigned __int128)result * numerator) / denom);
    }

    return result;
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until the upper bound exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r      = low;                         /* row index   */
    idx_t before = (unsigned __int128)r * (r + 1) / 2;  /* elements before this row */
    idx_t k      = n - before;                 /* position in the row */

    return binom_custom(r + 1, k);
}
