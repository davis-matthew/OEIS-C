/*
 * A144224  T(n,k) is the number of idempotent order-preserving full
 *          transformations (of an n-element chain) of waist k (waist(alpha) =
 *          max(Im(alpha))).
 * https://oeis.org/A144224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(2*d-1) where * m = greatest integer with
 *            m*(m+1)/2 <= n (triangular number method) * d = n - m*(m+1)/2 *
 *            For d = 0 the index is 2*m-1, otherwise it is 2*d-1. * The
 *            Fibonacci numbers are the standard sequence * F1 = 1, F2 = 1, F3
 *            = 2, F4 = 3,
 * Range:     u8 n <= 35, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find m: largest integer with T_m = m*(m+1)/2 <= n   */
    idx_t m   = 0;
    idx_t tri = 0;                      /* current triangular number T_m   */
    while (tri + (m + 1) <= n) {         /* next triangular number fits   */
        m  += 1;
        tri += m;                         /* tri = m*(m+1)/2                */
    }

    idx_t d = n - tri;                    /* distance from T_m              */

    idx_t fib_idx;
    if (d == 0)                            /* T_m itself: use 2*m-1          */
        fib_idx = 2 * m - 1;
    else
        fib_idx = 2 * d - 1;                /* otherwise 2*d-1                */

    /* compute Fibonacci(fib_idx) iteratively */
    if (fib_idx <= 2)
        return (seq_t)1;                   /* F1 and F2 are both 1           */

    seq_t f_prev = 1;                      /* F1                            */
    seq_t f_curr = 1;                      /* F2                            */
    for (idx_t i = 3; i <= fib_idx; ++i) {
        seq_t f_next = f_prev + f_curr;    /* F(i) = F(i-1)+F(i-2)          */
        f_prev = f_curr;
        f_curr = f_next;
    }
    return f_curr;                         /* F(fib_idx)                     */
}
