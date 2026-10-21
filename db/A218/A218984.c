/*
 * A218984  Power floor sequence of 2+sqrt(6).
 * https://oeis.org/A218984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 2*a(n-3); a(0) = 4, a(1) = 17, a(2)
 *            = 75
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of the power‑floor sequence for r = 2+sqrt(6). */
seq_t a(idx_t n)
{
    /* Directly return known initial values. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)75;

    /* Use three rolling variables to compute the recurrence in O(n). */
    seq_t a0 = (seq_t)4;   /* a(n-3) */
    seq_t a1 = (seq_t)17;  /* a(n-2) */
    seq_t a2 = (seq_t)75;  /* a(n-1) */
    seq_t ai;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (5 * a2) - (2 * a1) - (2 * a0);
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
