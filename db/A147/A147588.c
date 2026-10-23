/*
 * A147588  Duplicate of A070824.
 * https://oeis.org/A147588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = d(n) - 2, where d(n) is the number of positive divisors
 *            of n. For n=1 the sequence is defined to be 0. All primes
 *            therefore have value 0, and composites get the count of their
 *            proper non‑trivial divisors
 * Range:     i8 n <= 30, i16 n <= 30, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;           /* a(1) = 0 by definition */
    idx_t m = n;
    seq_t div_cnt = 1;              /* product of (exponent+1) for each prime factor */

    /* Trial division: for each potential factor p up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* count multiplicity of p */
            idx_t exp = 0;
            do
            {
                m /= p;
                ++exp;
            } while (m % p == 0);

            /* multiply divisors count by (exp + 1) */
            div_cnt *= (seq_t)(exp + 1);
        }
    }

    /* If m > 1, it is a prime factor > sqrt(n) with exponent 1 */
    if (m > 1)
        div_cnt *= 2;               /* factor (exp + 1) = (1 + 1) */

    /* a(n) = d(n) - 2  (exclude 1 and n) */
    seq_t result = div_cnt - 2;
    return result >= 0 ? result : 0;
}
