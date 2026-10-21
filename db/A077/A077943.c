/*
 * A077943  Expansion of 1/(1 - 2*x + 2*x^2 - 2*x^3).
 * https://oeis.org/A077943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3); a(0)=1, a(1)=2, a(2)=2
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 51, u64 n <= 102,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first three terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;

    /* Iterate using a sliding window of the last three values */
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)2; /* a(n-2) */
    seq_t a2 = (seq_t)2; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = 2*a2 - 2*a1 + 2*a0;   /* recurrence */
        a0 = a1;                    /* shift window */
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
