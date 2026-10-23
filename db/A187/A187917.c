/*
 * A187917  a(n) = (1/4)*(5*(n-1)*P(n)+n*P(n-1)) where P() are the Pell
 *          numbers A000129.
 * https://oeis.org/A187917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(5*(n-1)*P(n)+n*P(n-1)) (P = Pell numbers)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute Pell numbers P(n) and P(n-1) */
    seq_t p0 = 0;   /* P(0) */
    seq_t p1 = 1;   /* P(1) */
    if (n > 1) {
        for (idx_t i = 2; i <= n; ++i) {
            seq_t p = (seq_t)2 * p1 + p0;  /* P(i) = 2*P(i-1)+P(i-2) */
            p0 = p1;
            p1 = p;
        }
    }
    /* p1 = P(n),  p0 = P(n-1) */
    seq_t n_seq = (seq_t)n;
    seq_t term = ((seq_t)5 * (n_seq - 1) * p1 + n_seq * p0) / 4;
    return term;
}
