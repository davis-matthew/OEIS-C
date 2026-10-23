/*
 * A014544  Numbers k such that a cube can be divided into k subcubes.
 * https://oeis.org/A014544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 32 for n >= 16; for 1 <= n <= 15, the values are 1,
 *            8, 15, 20, 22, 27, 29, 34, 36, 38, 39, 41, 43, 45, 46
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the few special cases explicitly */
    if (n == 0)
        return (seq_t)0;                /* not used, but safe for n < 1 */
    if (n <= 15) {
        static const seq_t first[15] = {
            1, 8, 15, 20, 22,
            27, 29, 34, 36, 38,
            39, 41, 43, 45, 46
        };
        return first[n - 1];
    }
    /* for n >= 16 the sequence is simply n + 32 */
    return (seq_t)n + (seq_t)32;
}
