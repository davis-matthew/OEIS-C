/*
 * A142555  Primes congruent to 25 mod 53.
 * https://oeis.org/A142555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 25 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A142555)                                            */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the nth prime p (n ≥ 1) with p % 53 == 25.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1 */

    static seq_t *cache = NULL;       /* cached primes */
    static size_t cap   = 0;          /* allocated size */
    static size_t len   = 0;          /* number of primes stored */
    static idx_t  next  = 25;         /* next candidate ≡25 (mod 53) */

    /* Generate primes until we have at least n of them. */
    while (len < (size_t)n) {
        /* Skip non‑prime candidates. */
        while (!is_prime(next))
            next += 53;

        /* Grow cache if needed. */
        if (len == cap) {
            size_t newcap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)          /* allocation failure – unlikely */
                return 0;
            cache = tmp;
            cap = newcap;
        }

        cache[len++] = (seq_t)next;   /* store prime */
        next += 53;                   /* advance to the next ≡25 (mod 53) candidate */
    }

    return cache[(size_t)n - 1];
}
