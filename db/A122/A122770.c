/*
 * A122770  Numbers k such that A056109(k) is a square.
 * https://oeis.org/A122770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 4 with a(0)=0, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 0) return 0;
    if (n == 1) return 6;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 6;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 14*a(i-1) - a(i-2) + 4 */
        curr = 14 * prev1;
        curr -= prev2;
        curr += 4;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
