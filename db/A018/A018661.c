/*
 * A018661  Divisors of 814.
 * https://oeis.org/A018661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 814 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* --- precompute all divisors of 814 once --------------------------------- */
    static seq_t divisors[8];
    static int initialized = 0;
    if (!initialized)
    {
        /* factor 814 */
        idx_t x = 814;
        idx_t primes[32];
        idx_t pf = 0;
        for (idx_t p = 2; p * p <= x; ++p)
        {
            if (x % p == 0)
            {
                primes[pf++] = p;
                while (x % p == 0)
                    x /= p;
            }
        }
        if (x > 1)
            primes[pf++] = x;

        /* compute all divisors via exponent masks */
        idx_t num = ((idx_t)1 << pf);
        for (idx_t mask = 0; mask < num; ++mask)
        {
            seq_t d = 1;
            for (idx_t j = 0; j < pf; ++j)
                if (mask & ((idx_t)1 << j))
                    d *= (seq_t)primes[j];
            divisors[mask] = d;
        }

        /* sort the divisors in ascending order */
        for (idx_t i = 1; i < num; ++i)
        {
            seq_t key = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j - 1] > key)
            {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        initialized = 1;
    }

    /* --- answer the query ----------------------------------------------- */
    if (n == 0 || n > 8)
        return 0;           /* out of range: no further divisors */
    return divisors[n - 1];
}
