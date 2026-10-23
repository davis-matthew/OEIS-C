/*
 * A173130  a(n) = Cosh[(2 n - 1) ArcCosh[n]].
 * https://oeis.org/A173130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cosh((2 n - 1) * arccosh(n)) = ChebyshevT_{2n-1}(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the (2n-1)-th Chebyshev polynomial of the first kind evaluated at n.
   Uses the recurrence: T_0 = 1, T_1 = n, T_{k+1} = 2*n*T_k - T_{k-1}. */
seq_t a(idx_t n)
{
    /* Handle the OEIS offset: a(0) = 0. */
    if (n == 0)
        return 0;

    /* Desired Chebyshev order.  For n>=1 this is a positive integer. */
    idx_t order = 2 * n - 1;

    /* Base cases: T_0 = 1, T_1 = n. */
    seq_t t_prevprev = 1;   /* T_0 */
    seq_t t_prev     = (seq_t)n;  /* T_1 */

    /* If the required order is 0 or 1, return immediately. */
    if (order == 0) return t_prevprev;
    if (order == 1) return t_prev;

    /* Precompute the constant 2*n as a seq_t to avoid repeated casts. */
    seq_t two_n = ((seq_t)2 * (seq_t)n);

    /* Iterate the recurrence up to T_{order}. */
    seq_t cur;
    for (idx_t k = 2; k <= order; ++k) {
        cur = two_n * t_prev - t_prevprev;
        t_prevprev = t_prev;
        t_prev = cur;
    }

    return t_prev;
}
