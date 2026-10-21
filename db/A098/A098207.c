/*
 * A098207  a(n) is the square of near-repdigit number A033175(n).
 * https://oeis.org/A098207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((10^(n+1) - 7)/3)^2 Recurrence (valid for n > 2): a(0) =
 *            1, a(1) = 961, a(2) = 109561, a(n) = 111*a(n-1) - 1110*a(n-2) +
 *            1000*a(n-3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t a0 = (seq_t)1;
    static const seq_t a1 = (seq_t)961;
    static const seq_t a2 = (seq_t)109561;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* iterative computation using only the last three values */
    seq_t prev3 = a0;   /* a(n-3) */
    seq_t prev2 = a1;   /* a(n-2) */
    seq_t prev1 = a2;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = (seq_t)111  * prev1;
        seq_t t2 = (seq_t)1110 * prev2;
        seq_t t3 = (seq_t)1000 * prev3;
        seq_t val = t1 - t2 + t3;

        /* shift for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return prev1;
}
