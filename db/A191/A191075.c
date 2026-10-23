/*
 * A191075  Primes that are not squares mod 59.
 * https://oeis.org/A191075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which p is a quadratic nonresidue
 *            modulo 59
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence incrementally.  It keeps a dynamic
 * array of the primes found so far that are quadratic nonresidues of 59,
 * and advances a candidate counter as needed.  The lookup is O(1)
 * after the array is built. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* dynamic array of found non‑residue primes */
    static seq_t *nonres = NULL;
    static size_t nonres_len = 0;
    static size_t nonres_cap = 0;

    /* next prime candidate to test (uint64_t suffices for all requested
     * indices; the build time ensures that the required primes are
     * smaller than 2^64). */
    static uint64_t cur = 2;

    /* iterate until we have at least n terms stored */
    while (nonres_len < n)
    {
        /* test primality of the current candidate */
        if (is_prime(cur))
        {
            seq_t p   = (seq_t)cur;          /* cast to the build’s seq_t */
            seq_t base = p % (seq_t)59;        /* p mod 59  */
            seq_t res  = mod_pow(base, 29ULL, (seq_t)59); /* pow(base,29,59) */

            /* Legendre symbol (p/59) == -1 iff res == 58 (mod 59) */
            if (res == (seq_t)58)
            {
                /* grow storage if needed */
                if (nonres_len == nonres_cap)
                {
                    size_t new_cap = (nonres_cap == 0) ? 16 : nonres_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(nonres, new_cap * sizeof(seq_t));
                    nonres = tmp;
                    nonres_cap = new_cap;
                }
                nonres[nonres_len++] = p;
            }
        }

        /* advance to the next candidate: skip even numbers after 2 */
        cur += (cur == 2 ? 1 : 2);
    }

    return nonres[n - 1];
}
