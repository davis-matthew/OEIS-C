/*
 * A103640  Expansion of theta_4(q)^4 - theta_2(q)^4, where theta_2 and
 *          theta_4 are the Jacobi theta series.
 * https://oeis.org/A103640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = (-1)^n * 24 * σ_odd(n) (σ_odd(n) = sum of odd divisors of
 *            n)
 * Range:     i8 n <= 4, i16 n <= 734, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence A103640 is defined by the generating function
    theta_4(q)^4 - theta_2(q)^4.
    It can be expressed as a(n) = (-1)^n * A004011(n),
    where A004011(n) is the sum of the odd divisors of n.
    This implementation computes σ_odd(n) multiplicatively, removes
    all factors of 2 from n, and applies the sign (-1)^n.
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 by definition                      */

    /* remove power of 2:  n = 2^k * m ,  m odd                               */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;                  /* divide by 2 while even                      */

    /* compute σ_odd(m) = ∏_{p^e || m, p odd} (p^{e+1}-1)/(p-1)
       using integer arithmetic                                             */
    seq_t sigma = (seq_t)1;        /* product of prime–power sums                 */
    idx_t temp  = m;

    for (idx_t p = 3; p * p <= temp; p += 2)   /* test odd primes only         */
    {
        if (temp % p == 0)
        {
            seq_t prime_power_sum = (seq_t)1; /* 1 + p + p^2 + ... + p^e     */
            seq_t pow = (seq_t)1;             /* current p^k                  */
            while (temp % p == 0)
            {
                temp /= p;
                pow *= (seq_t)p;             /* p^{k+1}                      */
                prime_power_sum += pow;
            }
            sigma *= prime_power_sum;
        }
    }

    /* if a prime factor > sqrt(temp) remains, it is an odd prime with e=1 */
    if (temp > 1)
        sigma *= ((seq_t)1 + (seq_t)temp);

    /* combine with 24 and apply sign (-1)^n                                   */
    seq_t result = sigma * (seq_t)24;
    if ((n & 1) != 0)                 /* n odd  -> negative                    */
        result = -result;
    return result;
}
