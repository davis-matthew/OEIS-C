/*
 * A115245  Partial sums of A011764.
 * https://oeis.org/A115245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} 3^(2^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t term = 3;    /* term = 3^(2^k), starting with k = 0    */
    for (idx_t i = 0; ; ++i) {
        sum += term;            /* add current term          */
        if (i == n)             /* finished the summation    */
            break;
        term = seq_mul(term, term); /* next term: square it   */
    }
    return sum;
}
