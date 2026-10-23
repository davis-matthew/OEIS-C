/*
 * A129204  The denominator of 2/n^3.
 * https://oeis.org/A129204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(0) = 1; for n>0, a(2m) = 4*m^3, a(2m+1) = (2m+1)^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if ((n & 1) == 0) {                /* even n = 2m */
        seq_t m = (seq_t)(n >> 1);     /* m = n/2 */
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        return (seq_t)4 * m3;
    } else {                           /* odd n = 2m+1 */
        seq_t nn = (seq_t)n;
        seq_t nn2 = nn * nn;
        return nn2 * nn;
    }
}
