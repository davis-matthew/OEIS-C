/*
 * A063128  Dimension of the space of weight 2n cusp forms for Gamma_0( 60 ).
 * https://oeis.org/A063128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; a(n)=6*(4*n-3) for n>1
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1)=7, a(2)=30, a(n)=2*a(n-1)-a(n-2) for n>3 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)7;
    }
    /* For n > 1: a(n) = 6 * (4*n - 3) */
    seq_t nn = (seq_t)n;
    return (seq_t)6 * (((seq_t)4 * nn) - (seq_t)3);
}
