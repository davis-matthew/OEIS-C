/*
 * A107864  Duplicate of A099121.
 * https://oeis.org/A107864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = C( n*(n+5)/2 , n ), a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* top of the binomial coefficient */
    seq_t top = (seq_t)n * ((seq_t)n + 5) / 2;
    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t num = top - k + 1;          /* numerator term */

#if defined(__SIZEOF_INT128__)
        /* Use 128‑bit arithmetic when the underlying type is 64‑bit
           or smaller; for arbitrary precision types just use the
           native multiplication/division. */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 tmp = (unsigned __int128)res * (unsigned __int128)num;
            res = (seq_t)(tmp / k);
        }
        else
#endif
        {
            res = res * num;
            res /= k;
        }
    }

    return res;
}
