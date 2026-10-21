/*
 * A203309  Vandermonde determinant of the first n triangular numbers.
 * https://oeis.org/A203309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n‑1} k!·(2k+2)! / (2^k·(k+2)!) (with
 *            a(0)=a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Current product of the sequence so far */
    seq_t result = (seq_t)1;

    /* Helper values that are updated incrementally.
       fact_k      = k!
       fact_2k2    = (2k+2)!      (value for current k)
       fact_kp2    = (k+2)!      (value for current k)
       pow2_k      = 2^k         (value for current k)
    */
    seq_t fact_k      = (seq_t)1;          /* 1!   */
    seq_t fact_2k2    = (seq_t)24;         /* 4!   */
    seq_t fact_kp2    = (seq_t)6;          /* 3!   */
    seq_t pow2_k      = (seq_t)2;          /* 2^1  */

    for (idx_t k = 1; k < n; ++k)
    {
        /* Multiply by the current factor and divide by the denominator */
        seq_t numerator = fact_k * fact_2k2;
        seq_t denominator = pow2_k * fact_kp2;
        result = (result * numerator) / denominator;

        /* Prepare values for the next k (k -> k+1) */
        /* fact_k   becomes (k+1)!   */
        seq_t next_fact_k = fact_k * (seq_t)(k + 1);

        /* fact_2k2 becomes (2(k+1)+2)! = (2k+4)!   */
        seq_t next_fact_2k2 = fact_2k2;
        next_fact_2k2 *= (seq_t)(2 * k + 3);
        next_fact_2k2 *= (seq_t)(2 * k + 4);

        /* fact_kp2 becomes (k+3)!   */
        seq_t next_fact_kp2 = fact_kp2 * (seq_t)(k + 3);

        /* pow2_k becomes 2^(k+1)    */
        seq_t next_pow2_k = pow2_k * (seq_t)2;

        /* Commit updates for the next iteration */
        fact_k   = next_fact_k;
        fact_2k2 = next_fact_2k2;
        fact_kp2 = next_fact_kp2;
        pow2_k   = next_pow2_k;
    }

    return result;
}
