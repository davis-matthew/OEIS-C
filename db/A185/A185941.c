/*
 * A185941  First of a run of 4 or more consecutive primes which all equal 2
 *          (mod 3).
 * https://oeis.org/A185941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185941: first prime of a run of 4 consecutive primes all ≡ 2 (mod 3) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the n‑th term (n ≥ 1).  Terms are produced on demand and stored
   in a static cache; calls can come in any order. */
seq_t a(idx_t n)
{
    /* Static cache of sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Prime generator state */
    static uint64_t last_prime = 1;          /* last prime processed          */
    static int run_len = 0;                  /* current length of 2–mod‑3 run */
    static uint64_t ring[4];                 /* last 4 2‑mod‑3 primes          */
    static size_t ring_index = 0;            /* write position in ring        */

    /* Generate terms until we have at least n terms cached */
    while (cache_len < n) {
        /* Get the next prime after last_prime */
        uint64_t p = next_prime(last_prime);
        last_prime = p;

        if ((p % 3) == 2) {
            /* New prime is 2 mod 3 → extend current run */
            if (run_len == 0) {
                /* Start of a new run: reset buffer position */
                run_len = 1;
                ring_index = 0;
            } else {
                run_len++;
            }

            /* Put the prime into the circular buffer */
            ring[ring_index] = p;
            ring_index = (ring_index + 1) % 4;

            /* If we have at least 4 consecutive 2‑mod‑3 primes,
               the prime that is 3 positions behind is the start
               of a 4‑prime run. */
            if (run_len >= 4) {
                seq_t start = ring[ring_index];  /* element 3 steps before current */

                /* Grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }

                cache[cache_len++] = start;
            }
        } else {
            /* Non‑2‑mod‑3 prime → reset run state */
            run_len = 0;
            ring_index = 0;   /* clear buffer for next run */
        }
    }

    return cache[n - 1];
}
