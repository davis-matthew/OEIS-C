/*
 * A041149  Denominators of continued fraction convergents to sqrt(84).
 * https://oeis.org/A041149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 110*a(n-2) - a(n-4) Initial values: a(0) = 1, a(1) = 6,
 *            a(2) = 109, a(3) = 660
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)109;
    if (n == 3) return (seq_t)660;

    /* Rolling array of the last four computed terms. */
    seq_t stack[4];
    stack[0] = (seq_t)1;
    stack[1] = (seq_t)6;
    stack[2] = (seq_t)109;
    stack[3] = (seq_t)660;

    for (idx_t i = 4; i <= n; ++i) {
        /* i-2 and i-4 are always non‑negative here */
        stack[i & 3] = (seq_t)110 * stack[(i - 2) & 3] - stack[(i - 4) & 3];
    }
    return stack[n & 3];
}
