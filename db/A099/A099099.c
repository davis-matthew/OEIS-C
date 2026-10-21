/*
 * A099099  Quadrisection of a generalized Padovan sequence.
 * https://oeis.org/A099099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            1, a(2) = 1, a(3) = 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Sliding‑window variables: a0 = a(n‑4), a1 = a(n‑3), a2 = a(n‑2), a3 = a(n‑1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)1;  /* a(2) */
    seq_t a3 = (seq_t)2;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term using the recurrence */
        seq_t next = (seq_t)4 * a3 - (seq_t)6 * a2 + (seq_t)5 * a1 - a0;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
