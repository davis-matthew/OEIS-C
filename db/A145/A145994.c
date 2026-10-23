/*
 * A145994  Last prime in a run of at least 2 consecutive primes of the form
 *          4k+3.
 * https://oeis.org/A145994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145994: last prime in a run of at least 2 consecutive primes of the form 4k+3 */

#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_cnt = 0;

    /* Static state for prime generation */
    static idx_t next_candidate = 3;       /* next number to test for primality */
    static idx_t prev_prime = 2;         /* previous prime found (unused beyond state) */
    static int run_len = 0;              /* length of current consecutive 4k+3 run */
    static seq_t last_in_run = 0;        /* last prime of the current run */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_cnt < n) {
        /* Find next prime candidate */
        while (!is_prime(next_candidate))
            next_candidate += 2;          /* only odd numbers are tested */

        idx_t p = next_candidate;         /* current prime */
        bool is3 = ((p & 3ULL) == 3ULL);   /* p ≡ 3 (mod 4) */

        if (is3) {
            if (run_len > 0)
                run_len++;                 /* extend current run */
            else
                run_len = 1;               /* start new run */

            last_in_run = (seq_t)p;
        } else {
            if (run_len >= 2) {           /* run ended with a prime of this type */
                if (cache_cnt >= cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = last_in_run;
            }
            run_len = 0;                  /* reset run */
        }

        /* Prepare for next prime search */
        prev_prime = p;
        next_candidate += 2;
    }

    return cache[n - 1];
}
