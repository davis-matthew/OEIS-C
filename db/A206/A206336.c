/*
 * A206336  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 192, 96, 455, 296, 1376, 880, 4111,
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 32, u64 n <= 72, u128 n <= 154,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 7:
   a(n) = 872 * 3^((n-6)/2) + 6   if n is even
   a(n) = 12313 * 3^((n-9)/2) + 6 if n is odd */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)192,   /* a(1) */
        (seq_t)96,    /* a(2) */
        (seq_t)455,   /* a(3) */
        (seq_t)296,   /* a(4) */
        (seq_t)1376,  /* a(5) */
        (seq_t)880,   /* a(6) */
        (seq_t)4111   /* a(7) */
    };

    if (n <= 7) return init[n - 1];

    if ((n & 1ULL) == 0) {                /* even n */
        idx_t exp = (n - 6) / 2;
        return (seq_t)872 * pow3(exp) + (seq_t)6;
    } else {                              /* odd n */
        idx_t exp = (n - 9) / 2;
        return (seq_t)12313 * pow3(exp) + (seq_t)6;
    }
}
