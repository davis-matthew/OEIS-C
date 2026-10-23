/*
 * A119585  Triangle where n-th row is (1,2,3,...n) arranged first by number
 *          of divisors and then numerically (among those integers with the
 *          same number of divisors).
 * https://oeis.org/A119585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A119585: for each n, find the nth element of the triangle
   where each row r contains 1..r sorted by increasing number of divisors,
   ties resolved by numeric order. */
#include <stdlib.h>

static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 1, high = (n >> 1) + 1;          /* high > sqrt(n) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* ----- row and position within the row ----- */
    idx_t val = (idx_t)(((__int128)8 * n) - 7);
    idx_t r   = (isqrt_u64(val) + 1) / 2;        /* row number */

    idx_t offset = (idx_t)(((__int128)r * (r - 1)) / 2); /* sum_{i<r} i */
    idx_t k = n - offset;                          /* 1‑based */

    /* ----- divisor counts for 1..r ----- */
    idx_t *d = (idx_t *)malloc((r + 1) * sizeof(idx_t));
    if (!d)
        return (seq_t)0;          /* allocation failure */

    for (idx_t i = 1; i <= r; ++i)
        d[i] = 0;

    idx_t maxd = 0;
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j <= r; j += i)
            ++d[j];
        if (d[i] > maxd)
            maxd = d[i];
    }

    /* ----- find k-th element by divisor count order ----- */
    for (idx_t dval = 1; dval <= maxd; ++dval) {
        for (idx_t i = 1; i <= r; ++i) {
            if (d[i] == dval) {
                if (--k == 0) {
                    free(d);
                    return (seq_t)i;
                }
            }
        }
    }

    free(d);
    return (seq_t)0;   /* should not reach here */
}
