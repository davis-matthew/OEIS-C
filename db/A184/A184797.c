/*
 * A184797  Numbers k such that floor(k*sqrt(3)) is prime.
 * https://oeis.org/A184797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 1888, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184797: Numbers k such that ⌊k*√3⌋ is prime. */
/* Implementation: generate terms by testing k in order and caching results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t)              */
#include "../utils/is_prime_big.h" /* provides is_prime_big(seq_t)           */

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached k values                      */
    static size_t cache_len = 0;     /* number of terms stored              */
    static size_t cache_cap = 0;     /* capacity of cache array              */
    static idx_t k_next = 1;         /* next k to test                       */

    if (n == 0)                       /* undefined for offset < 1 */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* Find the next k with floor(k*sqrt(3)) prime */
        for (;;)
        {
            seq_t sq = (seq_t)3 * (seq_t)k_next * (seq_t)k_next;
            seq_t val = floor_sqrt(sq);           /* ⌊k*√3⌋          */
            if (is_prime_big(val))
            {
                /* Resize cache if necessary */
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k_next;  /* store k         */
                ++k_next;                     /* advance to next candidate */
                break;
            }
            ++k_next;
        }
    }

    return cache[n - 1];
}
