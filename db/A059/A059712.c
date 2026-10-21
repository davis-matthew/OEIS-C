/*
 * A059712  Number of stacked directed animals on the square lattice.
 * https://oeis.org/A059712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the variable‑coeff linear recurrence
 *            (105*n^2+861*n)*a(n)+(40*n^2+433*n+672)*a(n+1)-(55*n^2+586*n+1200)*a(n+2)+(10*n^2+112*n+288)*a(n+3)=0
 *            with initial values a(1)=1, a(2)=2, a(3)=6, a(4)=19
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)19;

    idx_t k_max = n - 3;          /* last k value to compute a(k+3)=a(n). */

    /* Window of three consecutive terms: a(k), a(k+1), a(k+2). */
    seq_t prev2 = (seq_t)2;   /* a(2)  */
    seq_t prev1 = (seq_t)6;   /* a(3)  */
    seq_t curr  = (seq_t)19;  /* a(4)  */

    for (idx_t k = 2; k <= k_max; ++k)
    {
        idx_t k2 = k * k;
        idx_t c0 = 105 * k2 + 861 * k;
        idx_t c1 = 40 * k2 + 433 * k + 672;
        idx_t c2 = 55 * k2 + 586 * k + 1200;
        idx_t c3 = 10 * k2 + 112 * k + 288;

        /* a(k+3) = (c2 * a(k+2) - c0 * a(k) - c1 * a(k+1)) / c3. */
        seq_t term1 = (seq_t)c2 * curr;
        seq_t term2 = (seq_t)c0 * prev2;
        seq_t term3 = (seq_t)c1 * prev1;
        seq_t numerator = term1 - term2 - term3;
        seq_t a_next = numerator / (seq_t)c3;

        /* Shift the window. */
        prev2 = prev1;
        prev1 = curr;
        curr  = a_next;
    }
    return curr;
}
