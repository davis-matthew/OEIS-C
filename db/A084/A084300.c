/*
 * A084300  a(n) = phi(n) mod 6.
 * https://oeis.org/A084300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = phi(n) mod 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined for n >= 1. Returns the Euler totient of n modulo 6. */

seq_t a(idx_t n)
{
    /* Factor n using trial division and compute phi(n) modulo 6.  */
    /* All arithmetic is performed with small integers modulo 6, so
       results fit into an int.  The final value is cast to seq_t. */
    if (n == 0) {          /* undefined for n = 0, return 0 as safe default */
        return (seq_t)0;
    }

    idx_t m = n;
    int res = 1;           /* phi(1) = 1, start with multiplicative identity */

    /* Trial division up to sqrt(m).  Use dividing by current p so that
       the loop terminates correctly even when m is reduced. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0)
            continue;

        /* Count exponent of prime p in factorization */
        int exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }

        /* Compute phi(p^exp) modulo 6: p^(exp-1) * (p-1) (mod 6) */
        int term;
        if (exp == 1)
        {
            term = (int)((p - 1) % 6);
        }
        else
        {
            /* p^(exp-1) mod 6 */
            int pow_mod = 1;
            int mod_p  = (int)(p % 6);
            for (int i = 0; i < exp - 1; ++i)
                pow_mod = (pow_mod * mod_p) % 6;

            term = (pow_mod * ((p - 1) % 6)) % 6;
        }

        res = (res * term) % 6;
    }

    /* If m > 1, it is a prime factor greater than sqrt(original n). */
    if (m > 1)
    {
        /* phi(m) = m-1 for prime m */
        int term = (int)((m - 1) % 6);
        res = (res * term) % 6;
    }

    return (seq_t)res;
}
