/*
 * A074377  Generalized 10-gonal numbers: m*(4*m - 3) for m = 0, +- 1, +- 2,
 *          +- 3, ...
 * https://oeis.org/A074377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * if n is even (n = 2k) : a(n) = k*(4*k + 3) * if n is
 *            odd (n = 2k+1) : a(n) = (2*k + 1)^2 + k * (equivalently
 *            a(2k)=k*(4k+3); a(2k+1)=(2k+1)^2+k)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                 /* n = 2k */
        seq_t kk = (seq_t)k;
        return kk * (((seq_t)4 * kk) + (seq_t)3);
    } else {
        idx_t k = (n - 1) >> 1;           /* n = 2k+1 */
        seq_t kk = (seq_t)k;
        seq_t m = (seq_t)(2 * k + 1);     /* m = 2k+1 */
        return m * m + kk;
    }
}
