/*
 * A119580  a(n) = (n^2+n^3)*binomial(2*n,n).
 * https://oeis.org/A119580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(n+1)*binomial(2*n,n) (OEIS A119580)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t base = binomial((idx_t)(2 * n), n);       /* binomial(2n,n) */
    seq_t nsq  = (seq_t)n * (seq_t)n;              /* n^2                */
    seq_t factor = nsq * ((seq_t)n + 1);           /* n^2*(n+1)          */
    return base * factor;                         /* final product      */
}
