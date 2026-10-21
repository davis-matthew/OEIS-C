/*
 * A097308  Chebyshev T-polynomials T(n,13) with Diophantine property.
 * https://oeis.org/A097308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chebyshev T-polynomials T(n,13)
   Recurrence: a(0) = 1, a(1) = 13,
                a(n) = 26*a(n-1) - a(n-2)   for n ≥ 2   */

/* Computes the n-th term of OEIS A097308. */
seq_t a(idx_t n) {
    /* Handle the two base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)13;

    /* Iterate the linear recurrence */
    seq_t a_prevprev = (seq_t)1;   /* a(n-2) */
    seq_t a_prev     = (seq_t)13;  /* a(n-1) */
    seq_t a_cur;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_cur = (seq_t)26 * a_prev - a_prevprev;
        a_prevprev = a_prev;
        a_prev     = a_cur;
    }
    return a_prev;
}
