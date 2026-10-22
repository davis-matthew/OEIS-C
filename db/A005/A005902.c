/*
 * A005902  Centered icosahedral (or cuboctahedral) numbers, also crystal ball
 *          sequence for f.c.c. lattice.
 * https://oeis.org/A005902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n+1)*(5*n^2+5*n+3)/3
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t two_n_plus_1 = 2 * nn + 1;
    seq_t poly = 5 * nn * nn + 5 * nn + 3;
    
    if (two_n_plus_1 % 3 == 0) {
        return (two_n_plus_1 / 3) * poly;
    } else {
        return two_n_plus_1 * (poly / 3);
    }
}
