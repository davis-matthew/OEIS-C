/*
 * A191072  Primes that are not squares mod 47.
 * https://oeis.org/A191072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191072: primes that are not squares modulo 47 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* fast modular exponentiation */
static uint32_t mod_pow_uint64(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint32_t result = 1 % mod;
    while (exp)
    {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset 1 – n=0 is outside the sequence */

    /* estimate upper bound for the n‑th non‑residue prime */
    uint32_t limit = (n < 4) ? 100 : n * 30;

    while (1)
    {
        size_t sz = (size_t)limit + 1;
        bool *is_composite = (bool *)calloc(sz, sizeof(bool));
        if (!is_composite)
            return 0;           /* allocation failure – unlikely */

        /* simple sieve of Eratosthenes */
        for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        {
            if (!is_composite[i])
            {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    is_composite[(size_t)j] = true;
            }
        }

        size_t count_nr = 0;    /* count of primes that are non‑residues mod 47 */
        for (uint32_t p = 2; p <= limit; ++p)
        {
            if (!is_composite[p])
            {
                uint32_t mod = p % 47;
                /* Legendre symbol (p|47) = mod^((47-1)/2) mod 47      */
                uint32_t leg = mod_pow_uint64(mod, 23, 47);
                if (leg == 47 - 1)   /* non‑residue */
                {
                    ++count_nr;
                    if (count_nr == n)
                    {
                        seq_t val = (seq_t)p;
                        free(is_composite);
                        return val;
                    }
                }
            }
        }

        free(is_composite);
        /* not enough primes – increase the limit and try again */
        limit *= 2;
    }
}
