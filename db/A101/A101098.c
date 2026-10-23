/*
 * A101098  a(1)=1; thereafter, a(n+1) = 20*n^3 + 10*n.
 * https://oeis.org/A101098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n)=20*(n-1)^3+10*(n-1) for n>1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 599, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;
    }
    seq_t m = (seq_t)(n - 1);   /* m = n-1 */
    seq_t m2 = m * m;           /* m^2 */
    seq_t m3 = m2 * m;          /* m^3 */
    return 20 * m3 + 10 * m;    /* 20*(n-1)^3 + 10*(n-1) */
}
