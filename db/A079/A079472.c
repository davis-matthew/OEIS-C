/*
 * A079472  Number of perfect matchings on an n X n L-shaped graph.
 * https://oeis.org/A079472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*F(n)*F(n-1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0; for n < 1 the OEIS offset is undefined, return 0 harmlessly */
    if (n < 1)
        return 0;

    /* Compute Fibonacci numbers F(n-1) and F(n) iteratively.
       Use the standard recurrence: F(0)=0, F(1)=1. */
    seq_t f_prev = 0;   /* F(0)   */
    seq_t f_curr = 1;   /* F(1)   */

    if (n > 1) {
        for (idx_t k = 2; k <= n; ++k) {
            seq_t f_next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_next;
        }
    }
    /* After the loop: f_prev = F(n-1), f_curr = F(n) (even for n==1) */

    seq_t res = f_prev * f_curr;  /* F(n-1)*F(n) */
    res *= 2;                     /* multiply by 2 */
    return res;
}
