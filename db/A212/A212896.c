/*
 * A212896  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)<2.
 * https://oeis.org/A212896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(n)=9*n^3-6*n^2+20*n-9 for n>1
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 781, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)16;
    }
    seq_t x = (seq_t)n;
    /* compute 9*x^3 - 6*x^2 + 20*x - 9 */
    seq_t term1 = (seq_t)9 * x * x * x;
    seq_t term2 = (seq_t)6 * x * x;
    seq_t term3 = (seq_t)20 * x;
    return term1 - term2 + term3 - (seq_t)9;
}
