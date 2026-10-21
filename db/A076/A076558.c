/*
 * A076558  a(n) = r * min(e_1, ..., e_r), where n = p_1^e_1 . .... p_r^e_r is
 *          the canonical prime factorization of n, a(1) = 0.
 * https://oeis.org/A076558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = r * min(e_1, …, e_r) where n = p_1^e_1 · … · p_r^e_r and
 *            a(1) = 0. This implementation factors n by trial division and
 *            tracks the number of distinct primes r and the smallest exponent
 *            min_e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;

    idx_t temp = n;          /* copy n so we may modify it */
    seq_t r = 0;             /* number of distinct prime factors */
    seq_t min_e = 0;         /* smallest exponent encountered */
    bool have_min = false;   /* true once a prime factor seen */

    /* factor 2 separately so the loop below can step by 2 */
    if ((temp & 1u) == 0u)
    {
        seq_t e = 0;
        while ((temp & 1u) == 0u)
        {
            temp >>= 1u;
            e++;
        }
        r++;
        if (!have_min || e < min_e)
        {
            min_e = e;
            have_min = true;
        }
    }

    /* factor odd primes */
    for (idx_t p = 3u; p * p <= temp; p += 2u)
    {
        if (temp % p == 0u)
        {
            seq_t e = 0;
            while (temp % p == 0u)
            {
                temp /= p;
                e++;
            }
            r++;
            if (!have_min || e < min_e)
            {
                min_e = e;
                have_min = true;
            }
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (temp > 1u)
    {
        r++;
        if (!have_min || (seq_t)1u < min_e)
            min_e = 1u;
    }

    return r * min_e;
}
