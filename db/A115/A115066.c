/*
 * A115066  Chebyshev polynomial of the first kind T(n,x), evaluated at x=n.
 * https://oeis.org/A115066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T_n(n) where T_n is Chebyshev polynomial of the first
 *            kind. T_0(x) = 1, T_1(x) = x, and T_{k+1}(x) = 2x*T_k(x) -
 *            T_{k-1}(x)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n == 0)
        return (seq_t)1;          /* T_0(0) = 1 */
    if (n == 1)
        return (seq_t)1;          /* T_1(1) = 1 */

    /* iterative computation of T_k(n) from k = 0 up to k = n. */
    seq_t t_prev = (seq_t)1;            /* T_0(n) */
    seq_t t_curr = (seq_t)n;            /* T_1(n) */
    seq_t twice_n = (seq_t)2 * (seq_t)n;/* 2 * n  */

    for (idx_t k = 1; k < n; ++k)
    {
        /* T_{k+1}(n) = 2 * n * T_k(n) - T_{k-1}(n). */
        seq_t t_next = twice_n * t_curr - t_prev;
        t_prev = t_curr;
        t_curr = t_next;
    }

    return t_curr;
}
