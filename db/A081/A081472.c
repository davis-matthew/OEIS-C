/*
 * A081472  a(n) = the smallest (n+k) such that the LCM of numbers from (n+1)
 *          to (n+k) is a multiple of n.
 * https://oeis.org/A081472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computes the smallest n+k such that lcm(n+1,…,n+k) is * a
 *            multiple of n. It is equivalent to * a(n) = n + max{ p^e - (n
 *            mod p^e) | p^e divides n, p prime, e≥1 }, * where a(1) = 2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Keep the original n for return */
    const idx_t orig = n;

    /* For n = 1 the loop below is skipped and max_t stays 1 */
    idx_t max_t = 1;
    idx_t temp = n;

    /* Factorise n.  For each prime power p^e obtain term = p^e. */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t term = 1;            /* term = p^e  */
            while (temp % p == 0)
            {
                temp /= p;
                term *= p;             /* increase exponent */
            }
            /* smallest t ≥ 1 such that term | (n + t) */
            idx_t t = term - (orig % term);
            if (t == 0)               /* n itself is divisible by term */
                t = term;
            if (t > max_t)
                max_t = t;
        }
    }

    /* Remaining prime factor > sqrt(n), if any */
    if (temp > 1)
    {
        idx_t term = temp;               /* prime factor to first power */
        idx_t t = term - (orig % term);
        if (t == 0)
            t = term;
        if (t > max_t)
            max_t = t;
    }

    /* The answer is n + the maximal required shift */
    return (seq_t)orig + (seq_t)max_t;
}
