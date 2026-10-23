/*
 * A008260  Coordination sequence for Paracelsian.
 * https://oeis.org/A008260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k) = 55k^2 + 2 for k>0, a(0)=1, a(5k+1) = 55k^2 + 22k + 4,
 *            a(5k+2) = 55k^2 + 44k + 10, a(5k+3) = 55k^2 + 66k + 21, a(5k+4)
 *            = 55k^2 + 88k + 37
 * Range:     u8 n <= 10, u16 n <= 172, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t k = n / 5;
    int r = n % 5;
    seq_t kk = (seq_t)k * (seq_t)k;          /* k^2 as seq_t */
    seq_t v;
    switch (r) {
    case 0:
        v = (seq_t)55 * kk + (seq_t)2;
        break;
    case 1:
        v = (seq_t)55 * kk + (seq_t)22 * (seq_t)k + (seq_t)4;
        break;
    case 2:
        v = (seq_t)55 * kk + (seq_t)44 * (seq_t)k + (seq_t)10;
        break;
    case 3:
        v = (seq_t)55 * kk + (seq_t)66 * (seq_t)k + (seq_t)21;
        break;
    case 4:
        v = (seq_t)55 * kk + (seq_t)88 * (seq_t)k + (seq_t)37;
        break;
    default:
        v = (seq_t)0;  /* unreachable */
    }
    return v;
}
