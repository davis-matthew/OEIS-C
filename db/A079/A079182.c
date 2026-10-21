/*
 * A079182  Number of non-commutative closed binary operations on a set of
 *          order n.
 * https://oeis.org/A079182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n^2) - n^((n^2 + n)/2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   int_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_pow_seq.h"

seq_t a(idx_t n)
{
    /* n must be treated as seq_t for arithmetic when raising to powers */
    if (n == 0) return (seq_t)0;

    seq_t base = (seq_t)n;
    idx_t exp1 = n * n;                           /* n^2  */
    idx_t exp2 = (n * n + n) / 2;                 /* (n^2 + n)/2 */

    seq_t term1 = int_pow_seq(base, exp1);
    seq_t term2 = int_pow_seq(base, exp2);

    return term1 - term2;
}
