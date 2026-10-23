/*
 * A038972  Primes p such that 85 is not a square mod p.
 * https://oeis.org/A038972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038972 – primes p such that 85 is not a quadratic residue modulo p */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/powmod64.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of primes and the sequence values */
    static seq_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t next_candidate = 2;   /* next integer to test for primality */

    /* Ensure we have enough terms for the requested n */
    while (seq_len < n)
    {
        /* Find the next prime */
        uint64_t p = next_candidate;
        next_candidate = (p == 2) ? 3 : p + 2;   /* advance to next odd */

        /* Skip even numbers larger than 2 */
        bool is_prime = true;
        if (p < 2) is_prime = false;
        else
        {
            for (size_t i = 0; i < primes_len; ++i)
            {
                uint64_t q = (uint64_t)primes[i];
                if (q * q > p) break;
                if (p % q == 0) { is_prime = false; break; }
            }
        }

        if (!is_prime) continue;

        /* Store prime */
        grow_seq(&primes, &primes_cap, primes_len + 1);
        primes[primes_len++] = p;

        /* Skip primes dividing 85 */
        if (p == 5 || p == 17) continue;

        /* Check if 85 is a quadratic non-residue mod p */
        uint64_t rem = powmod64(85ULL, (p - 1) >> 1, p);
        if (rem == p - 1)
        {
            /* Append to sequence */
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
