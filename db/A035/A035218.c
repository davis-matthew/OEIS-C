/*
 * A035218  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          36.
 * https://oeis.org/A035218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = d(6n) - d(3n) - d(2n) + d(n) = number of divisors of n
 *            that are NOT divisible by 2 or 3 = ∏_{p>3, p|n} (e_p + 1) where
 *            n = ∏ p^{e_p}
 * Range:     u8 n <= 9267, u16 n <= 9267, u32 n <= 9267, u64 n <= 9267,
 *            u128 n <= 9267, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 1 are undefined; here we simply return 0 */
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t temp = n;

    /* Strip all factors of 2 */
    while ((temp & 1) == 0)
        temp >>= 1;

    /* Strip all factors of 3 */
    while (temp % 3 == 0)
        temp /= 3;

    /* Factor remaining part (all primes > 3) */
    idx_t p = 5;
    while (p <= temp / p)          /* avoid overflow */
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }
            result *= (seq_t)(exp + 1);
        }
        p += 2; /* test only odd numbers (3 already removed) */
    }

    /* If a prime > 3 remains */
    if (temp > 1)
        result *= (seq_t)2;        /* exponent 1 -> factor (1+1) */

    return result;
}
