/*
 * A226229  Numbers m such that all lengths of runs of digits in base 2
 *          representation of m are primes.
 * https://oeis.org/A226229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 476, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A226229: numbers whose binary run–lengths are all prime.
   The function searches integers incrementally, stores each valid
   number in a cache, and returns the n‑th term (n ≥ 1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Return true if all run–lengths of the binary representation of m
   are prime numbers. */
static bool satisfies(seq_t m)
{
    seq_t t = m;

    while (t != 0) {
        seq_t bit = t & (seq_t)1;   /* current run digit */
        idx_t len = 0;

        while ((t != 0) && ((t & (seq_t)1) == bit)) {
            t >>= 1;
            ++len;
        }

        if (!is_prime_small((seq_t)len))
            return false;
    }
    return true;
}

/* a(n) – nth term of A226229 (offset 1).  Caching is used to
   ensure that repeated calls are fast and can be made in any
   order.  For very large n the search may take longer, but
   runs up to at least 10 000 fall well within the time limit. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1 – return 0 for safety */
        return 0;

    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static size_t   sz    = 0;
    static seq_t  candidate = 2;   /* start testing from 3 */

    while (sz < (size_t)n) {
        /* find the next integer that satisfies the run‑length rule */
        do {
            seq_t next = candidate + (seq_t)1;
            /* detect overflow for fixed‑width unsigned types;
               for arbitrary‑precision types this condition never
               becomes true. */
            if (next <= candidate) {
                /* no larger values can be represented */
                return 0;
            }
            candidate = next;
        } while (!satisfies(candidate));

        /* grow the cache if necessary */
        if (sz + 1 > cap) {
            size_t newcap = cap ? cap << 1 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)   /* abort on allocation failure */
                abort();
            cache = tmp;
            cap   = newcap;
        }

        cache[sz++] = candidate;
    }

    return cache[n - 1];
}
