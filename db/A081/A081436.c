/*
 * A081436  Fifth subdiagonal in array of n-gonal numbers A081422.
 * https://oeis.org/A081436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(2*n^2 + 3*n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1623, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                /* convert index to seq_t */
    seq_t term1 = sn + 1;               /* (n+1)          */
    seq_t sq    = sn * sn;              /* n^2            */
    seq_t inner = 2 * sq + 3 * sn + 2;  /* 2*n^2 + 3*n + 2 */
    return (term1 * inner) / 2;         /* final result   */
}
