/*
 * A061169  Third column of Lucas bisection triangle (even part).
 * https://oeis.org/A061169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060923(n+2, 2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n ≥ 6): a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) - a(n-6) */

seq_t a(idx_t n)
{
    /* Base cases for n = 0 … 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)39;
    if (n == 2) return (seq_t)315;
    if (n == 3) return (seq_t)1687;
    if (n == 4) return (seq_t)7470;
    if (n == 5) return (seq_t)29634;

    /* Store the six most recent values */
    seq_t prev[6] = { (seq_t)1, (seq_t)39, (seq_t)315, (seq_t)1687, (seq_t)7470, (seq_t)29634 };
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = 9 * prev[5] - 30 * prev[4] + 45 * prev[3]
              - 30 * prev[2] + 9 * prev[1] - prev[0];
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }
    return cur;
}
