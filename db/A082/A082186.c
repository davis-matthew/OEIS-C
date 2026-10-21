/*
 * A082186  1 + sum of first n terms of A001221.
 * https://oeis.org/A082186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} ω(k) (ω(k) = number of distinct prime
 *            factors of k)
 * Range:     u8 n <= 142, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *prefix = NULL;   /* prefix[i] = Σ_{k=1..i} ω(k) */
    static uint32_t *omega = NULL;
    static idx_t sieve_limit = 0;

    /* Extend sieve if needed */
    if (n > sieve_limit)
    {
        idx_t new_limit = n;
        /* Reallocate arrays */
        uint32_t *new_omega = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
        seq_t    *new_prefix = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
        if (!new_omega || !new_prefix)
            return 0; /* Out of memory – return 0 (unlikely in test) */

        /* initialise */
        for (idx_t i = 0; i <= new_limit; ++i)
            new_omega[i] = 0;
        new_prefix[0] = 0;

        /* Sieve of Eratosthenes – count distinct prime factors */
        for (idx_t i = 2; i <= new_limit; ++i)
        {
            if (new_omega[i] == 0)       /* prime */
            {
                for (idx_t j = i; j <= new_limit; j += i)
                    new_omega[j]++;      /* each multiple gains this prime */
            }
        }

        /* Build prefix sums */
        for (idx_t i = 1; i <= new_limit; ++i)
            new_prefix[i] = new_prefix[i - 1] + (seq_t)new_omega[i];

        /* Free old memory and update pointers */
        free(omega);
        free(prefix);
        omega = new_omega;
        prefix = new_prefix;
        sieve_limit = new_limit;
    }

    /* Return 1 + Σ_{k=1..n} ω(k) */
    return (seq_t)1 + prefix[n];
}
