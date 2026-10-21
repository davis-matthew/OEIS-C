/*
 * A212939  Number of nX4 0..3 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 100^n - 3*160^n + 256^n - 35^n + 2*80^n (offset 1, so
 *            a(1) = 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute the needed powers of small integers */
    seq_t pow100 = big_pow((seq_t)100, (seq_t)n);
    seq_t pow160 = big_pow((seq_t)160, (seq_t)n);
    seq_t pow256 = big_pow((seq_t)256, (seq_t)n);
    seq_t pow35  = big_pow((seq_t)35,  (seq_t)n);
    seq_t pow80  = big_pow((seq_t)80,  (seq_t)n);

    /* assemble the linear combination */
    seq_t result = pow100;          /* +100^n */
    result -= 3 * pow160;          /* -3*160^n */
    result += pow256;              /* +256^n */
    result -= pow35;               /* -35^n */
    result += 2 * pow80;           /* +2*80^n */

    return result;
}
