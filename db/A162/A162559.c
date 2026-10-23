/*
 * A162559  a(n) = ((4+sqrt(3))*(1+sqrt(3))^n + (4-sqrt(3))*(1-sqrt(3))^n)/2.
 * https://oeis.org/A162559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(0) = 4, a(1) = 7
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == (idx_t)0) return (seq_t)4;
    if (n == (idx_t)1) return (seq_t)7;

    /* Variables to hold two previous terms.
       They are initialized with the known values a(0) and a(1). */
    seq_t a_prev2 = (seq_t)4;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)7;   /* a(n-1) */
    seq_t a_curr  = 0;          /* a(n) */

    /* Compute successive terms up to the requested n.
       The loop runs exactly n-1 iterations for n >= 2. */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_curr = a_prev1 * (seq_t)2 + a_prev2 * (seq_t)2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
