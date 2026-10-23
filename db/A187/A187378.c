/*
 * A187378  Number of 4-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 27*n^2 - 97*n + 88 for n > 2
 * Special values: a(1) = 0, a(2) = 4
 */
seq_t a(idx_t n)
{
    if (n <= 2) {
        /* a(1) = 0, a(2) = 4; a(0) is undefined for the sequence but return 0 */
        return (n == 1) ? 0 : (n == 2) ? 4 : 0;
    }
    seq_t nn = (seq_t)n;
    seq_t result = 27 * nn * nn - 97 * nn + 88;
    return result;
}
