/*
 * A197424  Number of subsets of {1, 2, ..., 4*n + 2} which do not contain two
 *          numbers whose difference is 4.
 * https://oeis.org/A197424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+2)*F(n+3))^2 where F(k) is the k-th Fibonacci number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Iteratively compute Fibonacci numbers up to F(n+3). */
    seq_t prev = (seq_t)0;    /* F(0) */
    seq_t cur  = (seq_t)1;    /* F(1) */
    seq_t f_n_plus_2 = 0;     /* to hold F(n+2) */
    seq_t f_n_plus_3 = 0;     /* to hold F(n+3) */

    /* Loop from i = 0 to i = n+1 inclusive (total n+2 iterations). */
    for (idx_t i = 0; i < n + 2; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur = next;

        if (i == n)      f_n_plus_2 = cur;   /* cur == F(n+2) after i==n */
        if (i == n + 1)  f_n_plus_3 = cur;   /* cur == F(n+3) after i==n+1 */
    }

    /* Compute a(n) = (F(n+2) * F(n+3))^2 */
    seq_t prod = f_n_plus_2 * f_n_plus_3;
    return prod * prod;
}
