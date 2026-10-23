/*
 * A107385  a(n) = a(n-1)+4*a(n-2)-4*a(n-4).
 * https://oeis.org/A107385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-4); Initial conditions: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the 4 known initial values directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;

    /* Values for the last four terms:
         v0 = a(i-4), v1 = a(i-3), v2 = a(i-2), v3 = a(i-1).   */
    seq_t v0 = 0, v1 = 1, v2 = 1, v3 = 2, v4;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-2) - 4*a(i-4) */
        v4 = v3 + (seq_t)4 * v2 - (seq_t)4 * v0;

        /* Shift window for next iteration. */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
    }
    return v3; /* v3 holds a(n) after the loop. */
}
