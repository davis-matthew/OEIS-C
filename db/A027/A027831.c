/*
 * A027831  Expansion of 1/(1 - 4*x + 2*x^2 + 4*x^3 - 2*x^4).
 * https://oeis.org/A027831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) + 2*a(n-4),
   initial values: a(0) = 1, a(1) = 4, a(2) = 14, a(3) = 44. */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)44;

    /* Rolling variables to keep the last four terms */
    seq_t a0 = (seq_t)1, a1 = (seq_t)4, a2 = (seq_t)14, a3 = (seq_t)44;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)4 * a3
            - (seq_t)2 * a2
            - (seq_t)4 * a1
            + (seq_t)2 * a0;
        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
