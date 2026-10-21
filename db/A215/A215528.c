/*
 * A215528  Slowest increasing sequence of alternate-parity integers m such
 *          that 2m^2+1 is prime.
 * https://oeis.org/A215528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 19, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A215528 – slowest increasing sequence of alternate‑parity integers
   m such that 2*m^2+1 is prime.  Index a(1)=1.  The implementation
   memoises previously computed terms and satisfies any ordering of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)             /* undefined, but guard against accidental use */
        return (seq_t)0;
    if (n <= cache_len)     /* already computed */
        return cache[n - 1];

    /* Ensure the first term a(1)=1 exists */
    if (cache_len == 0) {
        if (!cache_cap) {
            cache_cap = 1;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Generate terms until we have the n‑th term */
    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + (seq_t)1;   /* opposite parity to prev */

        /* Search for the next valid cand */
        while (1) {
            seq_t val = cand * cand;   /* cand^2 */
            val = val * (seq_t)2;      /* 2*cand^2 */
            val = val + (seq_t)1;      /* 2*cand^2 + 1 */

            if (is_prime_big(val))
                break;                /* cand satisfies the condition */

            cand += (seq_t)2;          /* keep alternate parity */
        }

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)                 /* allocation failure */
                exit(EXIT_FAILURE);
            cache     = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = cand;
        cache_len++;
    }

    return cache[n - 1];
}
