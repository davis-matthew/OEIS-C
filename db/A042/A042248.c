/*
 * A042248  Numerators of continued fraction convergents to sqrt(650).
 * https://oeis.org/A042248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 102*a(n-2) - a(n-4) with a(0)=25, a(1)=51, a(2)=2575,
 *            a(3)=5201
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)25;
    if (n == 1) return (seq_t)51;
    if (n == 2) return (seq_t)2575;
    if (n == 3) return (seq_t)5201;

    seq_t buf[4] = {(seq_t)25, (seq_t)51, (seq_t)2575, (seq_t)5201};

    for (idx_t k = 4; k <= n; ++k) {
        idx_t i_n   = k & 3;
        idx_t i_nm2 = (k - 2) & 3;
        idx_t i_nm4 = (k - 4) & 3;
        buf[i_n] = 102 * buf[i_nm2] - buf[i_nm4];
    }
    return buf[n & 3];
}
