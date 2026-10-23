/*
 * A008239  Coordination sequence T2 for Zeolite Code THO.
 * https://oeis.org/A008239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(24*m + k) = 1152*m^2 + 96*k*m + c[k] where c[0] = 1, c[1] = 4,
 *            … , c[23] = 1060, but for k == 0 and m > 0 an extra 1 is added.
 *            This yields a(24)=1154, a(744)=1107074, a(840)=1411202, etc
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t c[24] = {
        1, 4, 9, 19, 33, 50, 74, 100, 129, 165,
        201, 242, 290, 338, 393, 453, 513, 580, 650, 722,
        801, 883, 969, 1060
    };
    idx_t m = n / 24;
    idx_t k = n % 24;
    seq_t val = (seq_t)1152 * (seq_t)m * (seq_t)m;
    val += (seq_t)96 * (seq_t)k * (seq_t)m;
    val += c[k];
    if (k == 0 && m > 0) {
        val += (seq_t)1;
    }
    return val;
}
