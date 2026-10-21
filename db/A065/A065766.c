/*
 * A065766  Sum of divisors of twice a square number, divided by three.
 * https://oeis.org/A065766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(2*n^2)/3 (sum of divisors of 2·n² divided by 3)
 * Range:     u8 n <= 8, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the exponent of 2 in n */
    idx_t temp = n;
    idx_t e2 = 0;
    while (temp % 2 == 0) {
        temp /= 2;
        ++e2;
    }

    /* Sigma(2*n^2) = product over primes of (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;

    /* Handle odd prime factors of n */
    idx_t p = 3;
    while (p * p <= temp) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* exponent of p in 2*n^2 is 2*e */
            idx_t exp = e * 2;

            seq_t term = 1;
            seq_t cur  = 1;
            for (idx_t i = 0; i < exp; ++i) {
                cur *= (seq_t)p;
                term += cur;
            }
            sigma *= term;
        }
        p += 2; /* skip even numbers */
    }

    /* Remaining prime factor > sqrt(temp), if any */
    if (temp > 1) {
        /* temp itself is a prime factor with exponent 1 */
        idx_t exp = 2; /* 2*1 */
        seq_t term = 1;
        seq_t cur  = 1;
        for (idx_t i = 0; i < exp; ++i) {
            cur *= (seq_t)temp;
            term += cur;
        }
        sigma *= term;
    }

    /* Add the factor 2, which is always present in 2*n^2.
       Its exponent is 2*e2 + 1. */
    idx_t exp2 = e2 * 2 + 1;
    seq_t term2 = 1;
    seq_t cur2  = 1;
    for (idx_t i = 0; i < exp2; ++i) {
        cur2 *= (seq_t)2;
        term2 += cur2;
    }
    sigma *= term2;

    /* Final result is sigma divided by 3 */
    return sigma / 3;
}
