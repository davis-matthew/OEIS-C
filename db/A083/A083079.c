/*
 * A083079  4th column of number array A083075.
 * https://oeis.org/A083079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^3 + 28*n^2 + 26*n + 1
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 811, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          // n^2
    seq_t n3 = n2 * sn;          // n^3
    seq_t res = 8 * n3;          // 8*n^3
    res += 28 * n2;              // +28*n^2
    res += 26 * sn;              // +26*n
    res += 1;                    // +1
    return res;
}
