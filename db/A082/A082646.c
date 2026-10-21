/*
 * A082646  Primes whose decimal expansions contain equal numbers of each of
 *          their digits.
 * https://oeis.org/A082646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 2273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7828, big any n
 * Helpers:   digit_counts, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A082646:  a(n) is the n‑th prime whose decimal
   representation uses each of its digits an equal number of times.
   Example: 127 is good (1,2,7 each once); 101 is not (two 1's, one 0). */

/* Helper functions from the utils library ----------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrime.h"      /* bool isPrime(idx_t x); */
#include "../utils/digit_counts.h"/* void digit_counts(seq_t x, unsigned char cnt[10]); */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* next candidate after the one that was already tested last time */
    static idx_t next_candidate = 2;

    /* grow the cache if needed */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* find terms until we have at least n of them */
    while (cache_len < n) {
        idx_t candidate = next_candidate;

        /* advance next_candidate to the following odd number
           (after 2 the next is 3, thereafter candidate+2) */
        if (candidate == 2)
            next_candidate = 3;
        else
            next_candidate = candidate + 2;

        /* test primality */
        if (!isPrime(candidate))
            continue;

        /* count digits of candidate */
        unsigned char cnt[10] = {0};
        digit_counts((seq_t)candidate, cnt);

        /* check that all non‑zero counts are equal */
        bool ok   = true;
        int  freq = 0;          /* frequency seen for the first digit that appears */
        for (int d = 0; d < 10 && ok; ++d) {
            if (cnt[d]) {
                if (freq == 0)
                    freq = cnt[d];
                else if (cnt[d] != freq)
                    ok = false;
            }
        }

        if (ok)
            cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n - 1];
}
