/*
 * A222468  Numerator sequence of the n-th convergent of the continued
 *          fraction 1/(1+2/(2+2/(3+2/(4+...
 * https://oeis.org/A222468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + 2*a(n-2) for n>=3 * with initial values
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offsets below 1 are irrelevant for the OEIS entry. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* iterative dynamic‑programming using the recurrence */
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)2;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = (seq_t)i * a_prev1 + (seq_t)2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = term;
    }
    return a_prev1;
}
