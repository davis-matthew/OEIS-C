/*
 * A139595  A139277(n) followed by A139273(n+1).
 * https://oeis.org/A139595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2k) = 8*k^2 + 5*k Formula: a(2k+1) = 8*(k+1)^2 - 3*(k+1) (k =
 *            ⌊n/2⌋)
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* even index: n = 2k */
        seq_t k = (seq_t)(n / 2);
        seq_t r = k * k;
        r *= 8;
        r += 5 * k;
        return r;
    } else {
        /* odd index: n = 2k+1, use m = k+1 = (n+1)/2 */
        seq_t m = (seq_t)((n + 1) / 2);
        seq_t r = m * m;
        r *= 8;
        r -= 3 * m;
        return r;
    }
}
