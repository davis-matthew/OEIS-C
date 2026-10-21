/*
 * A088229  Number of n X n (0,1) matrices with distinct rows.
 * https://oeis.org/A088229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * binomial(2^n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention */
    if (n == 0) return (seq_t)1;

    /* Compute 2^n in seq_t (avoid overflow of fixed‑width types) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 = pow2 * (seq_t)2;

    /* Compute binomial(2^n , n) multiplicatively */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = pow2 - (seq_t)(n - i);
        binom = (binom * numerator) / (seq_t)i;
    }

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact = fact * (seq_t)i;

    return fact * binom;
}
