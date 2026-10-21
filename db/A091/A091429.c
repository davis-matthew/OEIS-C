/*
 * A091429  Numerator of a(n) = (integral_{x=0..1/3} (1-x^2)^n dx).
 * https://oeis.org/A091429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the integral definition:
   a(0) = 1
   a(n) = 18*n*a(n-1) + (2n+2)*(2n)!*2^(2n-1)/(n+1)!   for n >= 1
   The recurrence is implemented only with integer operations, so
   the result is exact for every n. */

seq_t a(idx_t n)
{
    seq_t a_prev = (seq_t)1;        /* a(0) */
    if (n == (idx_t)0)
        return a_prev;

    seq_t P = (seq_t)1;            /* P(1) = (2*1)!/(1+1)! = 1 */
    seq_t pow2 = (seq_t)2;         /* 2^(2*1-1) = 2 */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* Update P and pow2 for n >= 2 */
        if (i > (idx_t)1)
        {
            /* P(i) = P(i-1) * (2i)*(2i-1) / (i+1) */
            seq_t tmp = P * (seq_t)(2*i) * (seq_t)(2*i-1);
            P = tmp / (seq_t)(i+1);

            /* pow2(i) = pow2(i-1) * 4  i.e. 2^(2i-1) */
            pow2 *= (seq_t)4;
        }

        /* Term T = (2i+2) * pow2 * P */
        seq_t T = (seq_t)(2*i+2) * pow2 * P;

        /* a(i) = 18*i*a(i-1) + T */
        seq_t term = a_prev * (seq_t)(18*i);
        a_prev = term + T;
    }

    return a_prev;
}
