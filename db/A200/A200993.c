/*
 * A200993  Triangular numbers, T(m), that are two-thirds of another
 *          triangular number; T(m) such that 3*T(m) = 2*T(k) for some k.
 * https://oeis.org/A200993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 10, for n > 1: a(n) = 98*a(n-1) - a(n-2) + 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)10;

    /* Iterate the recurrence to reach the desired index. */
    seq_t a_prev_prev = (seq_t)0;  /* a(0) */
    seq_t a_prev      = (seq_t)10; /* a(1) */
    seq_t a_curr;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_curr = (seq_t)98 * a_prev - a_prev_prev + (seq_t)10;
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }

    /* After the loop, a_prev holds a(n). */
    return a_prev;
}
