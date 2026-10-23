/*
 * A134371  a(n) = ((2n)!)^n.
 * https://oeis.org/A134371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)!)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t m = 2 * n;
    seq_t f = factorial(m);          /* compute (2n)! */
    seq_t res = seq_pow(f, n);       /* raise it to the n-th power */
    return res;
}
