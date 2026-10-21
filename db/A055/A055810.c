/*
 * A055810  a(n) = T(n,n-5), array T as in A055807.
 * https://oeis.org/A055810
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (240 -54*n -49*n^2 +6*n^3 +n^4)/24 for n > 5, a(5)=1
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 291, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 5) return 1;
    if (n < 5) return 0;   /* value not defined for offsets below 5 */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t val = ((seq_t)240
                 - (seq_t)54 * nn
                 - (seq_t)49 * n2
                 + (seq_t)6  * n3
                 + n4) / 24;
    return val;
}
