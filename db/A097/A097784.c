/*
 * A097784  Partial sums of Chebyshev sequence S(n,10) = U(n,5) =
 *          A004189(n+1).
 * https://oeis.org/A097784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) + 1; a(0) = 1, a(1) = 11 Recurrence
 *            derived from the OEIS entry for A097784
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Compute iteratively up to n; no recursion needed. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)11;  /* a(1) */
    seq_t a2;             /* temporary for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = 10 * a1 - a0 + 1; /* recurrence */
        a0 = a1;
        a1 = a2;
    }

    return a1; /* a(n) */
}
