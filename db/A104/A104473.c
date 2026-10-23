/*
 * A104473  a(n) = binomial(n+2,2)*binomial(n+6,2).
 * https://oeis.org/A104473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2) * binomial(n+6,2)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 358, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute binomial(n+2,2) = (n+2)*(n+1)/2 */
    seq_t bin1_n1 = (seq_t)n + 2;
    seq_t bin1_n2 = (seq_t)n + 1;
    if ((bin1_n1 & 1) == 0)     /* bin1_n1 even */
        bin1_n1 >>= 1;          /* divide the even part by 2 */
    else
        bin1_n2 >>= 1;          /* else divide the other part */

    seq_t bin1 = bin1_n1 * bin1_n2;

    /* compute binomial(n+6,2) = (n+6)*(n+5)/2 */
    seq_t bin2_n1 = (seq_t)n + 6;
    seq_t bin2_n2 = (seq_t)n + 5;
    if ((bin2_n1 & 1) == 0)
        bin2_n1 >>= 1;
    else
        bin2_n2 >>= 1;

    seq_t bin2 = bin2_n1 * bin2_n2;

    return bin1 * bin2;
}
