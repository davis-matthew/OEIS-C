/*
 * A215379  Primes congruent to {0, 1, 2, 3} mod 13.
 * https://oeis.org/A215379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p mod 13 ≤ 3 (i.e., p ≡ 0,1,2,3 (mod
 *            13))
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 897 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found terms                */
    static size_t  cap   = 0;        /* allocated capacity                  */
    static idx_t   len   = 0;        /* number of terms cached              */
    static uint64_t last = 0;        /* last prime found (for candidate gen) */

    /* Ensure the cache contains at least the nth term. */
    while (len < n)
    {
        uint64_t candidate;

        /* Define the starting candidate for the next search. */
        if (len == 0)                 /* first prime is 2 */
            candidate = 2;
        else
        {
            candidate = last + 1;     /* start just after the last found prime */
            if ((candidate & 1) == 0) /* skip even numbers (except 2) */
                ++candidate;
        }

        /* Search for the next prime that satisfies the residue constraint. */
        for (;; candidate += 2)       /* iterate only over odd numbers          */
        {
            if ((candidate % 13) <= 3 && is_prime_seq((seq_t)candidate))
            {
                /* Grow the cache array if necessary. */
                if (cap == len)
                {
                    size_t newcap = cap == 0 ? 32 : cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, newcap);
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = newcap;
                }

                cache[len++] = (seq_t)candidate;
                last = candidate;
                break;              /* prime found; proceed to next term   */
            }
        }
    }

    return cache[n - 1];
}
