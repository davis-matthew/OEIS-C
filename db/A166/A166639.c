/*
 * A166639  a(n) = 6*n + 3 + 4*(-1)^n.
 * https://oeis.org/A166639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 3 + 4*(-1)^n
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * 6;
    if ((n & 1) == 0) {
        /* even n: 6*n + 7 */
        res += 7;
    } else {
        /* odd n: 6*n - 1 */
        res -= 1;
    }
    return res;
}
