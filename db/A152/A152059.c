/*
 * A152059  a(n) is the number of ways 2n-1 seats can be occupied by at most n
 *          people for n>=1, with a(0)=1.
 * https://oeis.org/A152059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * C(2*n-1,k) * C(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t fact = 1;      /* k!   */
    seq_t binom_n = 1;   /* C(n,k)      */
    seq_t binom_m = 1;   /* C(2*n-1,k)  */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = fact * binom_m * binom_n;
        sum += term;

        if (k == n) break;                 /* no next step needed */
        fact = fact * (seq_t)(k + 1);
        binom_n = binom_n * (seq_t)(n - k) / (seq_t)(k + 1);
        binom_m = binom_m * (seq_t)(2 * n - 1 - k) / (seq_t)(k + 1);
    }
    return sum;
}
