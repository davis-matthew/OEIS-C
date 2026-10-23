/*
 * A128889  a(n) = (2^(n^2) - 1)/(2^n - 1).
 * https://oeis.org/A128889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n^2) - 1)/(2^n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* below the sequence offset – value arbitrary */

    idx_t exp = n * n;            /* compute n^2 safely in idx_t */
    seq_t num = pow2(exp) - 1;    /* 2^(n^2) - 1 */
    seq_t den = pow2(n)  - 1;     /* 2^n - 1 */
    return num / den;             /* exact division */
}
