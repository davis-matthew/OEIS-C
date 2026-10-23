/*
 * A107713  Convolution of 2^n*n! and n!.
 * https://oeis.org/A107713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * k! * (n-k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t result = 0;
    seq_t fact_k = (seq_t)1;        /* k!  (starts with 0! = 1) */
    seq_t pow2   = (seq_t)1;        /* 2^k (starts with 2^0 = 1) */

    /* compute n! once: curr_fact = (n-k)! for the current k   */
    seq_t curr_fact = (seq_t)1;
    for (i = 1; i <= n; ++i)
        curr_fact *= (seq_t)i;      /* curr_fact = n! initially */

    /* sum over k = 0 .. n-1, update curr_fact for next k   */
    for (i = 0; i < n; ++i)
    {
        seq_t term = fact_k * pow2;
        term *= curr_fact;         /* term = (k! * 2^k) * (n-k)! */
        result += term;

        /* prepare for next k */
        fact_k *= (seq_t)(i + 1);  /* fact_k becomes (i+1)!   */
        pow2 <<= 1;                /* pow2 becomes 2^(i+1)    */
        curr_fact /= (seq_t)(n - i);/* curr_fact becomes (n-i-1)! */
    }

    /* final term k = n (now curr_fact == 1) */
    {
        seq_t term = fact_k * pow2;/* fact_k is n!, pow2 is 2^n */
        result += term;
    }

    return result;
}
