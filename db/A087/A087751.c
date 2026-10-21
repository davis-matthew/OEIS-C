/*
 * A087751  Weighted sum of the harmonic numbers.
 * https://oeis.org/A087751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1) + (n-1)!*(2^n-1) (a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (i-1)! and 2^(i-1) are stored in fact_prev and pow2_prev.
       a_val holds a(i-1) during the loop. */
    seq_t fact_prev = 1; /* 0!   */
    seq_t pow2_prev = 1; /* 2^0  */
    seq_t a_val    = 0;  /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* 2^i */
        seq_t pow2_cur = pow2_prev * (seq_t)2;

        /* term = (i-1)! * (2^i - 1) */
        seq_t term = fact_prev * (pow2_cur - (seq_t)1);

        /* a(i) = 2*i*a(i-1) + term */
        seq_t mult = (seq_t)2 * (seq_t)i * a_val;
        a_val = mult + term;

        /* Update factorial and power of two for next iteration */
        fact_prev *= (seq_t)i;   /* fact_prev becomes i!   */
        pow2_prev = pow2_cur;    /* pow2_prev becomes 2^i  */
    }

    return a_val;
}
