/*
 * A186154  Rank of (1/8)n^3 when {(1/8)i^3: i>=1} and {j^2>: j>=1} are
 *          jointly ranked with (1/8)i^3 after j^2 when (1/8)i^3=j^2.
 *          Complement of A186155.
 * https://oeis.org/A186154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) + floor_sqrt((n^3-1)/8) + 1 + [ n^3/8 is a perfect
 *            square integer ]
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 */
    seq_t n_t = (seq_t)n;
    seq_t n3 = n_t * n_t * n_t;           /* n^3  */
    seq_t t_minus1 = n3 - 1;              /* n^3-1  */
    seq_t t_div8  = t_minus1 / 8;         /* (n^3-1)/8  */
    seq_t sqt = floor_sqrt(t_div8);       /* floor(sqrt((n^3-1)/8)) */
    seq_t rank = (seq_t)(n - 1) + sqt + 1;    /* base rank */

    /* indicator for presence of an earlier equal square (n^3/8 integer square) */
    if ((n3 % 8) == 0) {                  /* n^3 divisible by 8 -> X integer */
        seq_t X = n3 / 8;                 /* X = n^3/8  */
        seq_t xr = floor_sqrt(X);         /* sqrt floor of X */
        if (xr * xr == X)
            rank += 1;                   /* add one more for the earlier square */
    }

    return rank;
}
