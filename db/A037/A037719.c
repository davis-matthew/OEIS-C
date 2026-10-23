/*
 * A037719  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2; a(n) = 7*a(n-1) + d_n, where d_n = {2,0,1,3} repeating
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; return 0 for safety */
        return 0;

    static const seq_t pattern[4] = { 2, 0, 1, 3 };
    seq_t result = pattern[0];        /* a(1) = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* index into the repeating pattern: (i-1) % 4 */
        seq_t d = pattern[(i - 1) & 3u];
        result = 7 * result + d;
    }
    return result;
}
