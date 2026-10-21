/*
 * A054811  Fourth term of strong prime quintets: p(m-2)-p(m-3) >
 *          p(m-1)-p(m-2) > p(m)-p(m-1) > p(m+1)-p(m).
 * https://oeis.org/A054811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the 4th prime of each consecutive quintet of primes that
 *            satisfies gap1 > gap2 > gap3 > gap4 (strictly decreasing gaps)
 * Range:     u8 none, u16 n <= 171, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc_new_array(size_t n); */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_cnt = 0;

    /* Sliding window of the last 5 primes. */
    static seq_t buf[5];
    static size_t buf_cnt = 0;

    /* Next starting point for next_prime. */
    static uint64_t next_start = 2;

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        /* Get next prime. */
        uint64_t p_u = next_prime(next_start);
        next_start = p_u + 1;                 /* sequence of primes is strictly increasing */

        /* Insert into buffer. */
        if (buf_cnt < 5) {
            buf[buf_cnt++] = (seq_t)p_u;
        } else {
            /* Shift left and append new prime. */
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            buf[3] = buf[4];
            buf[4] = (seq_t)p_u;
        }

        /* Once we have 5 primes, test the decreasing-gap condition. */
        if (buf_cnt == 5) {
            seq_t g1 = buf[1] - buf[0];
            seq_t g2 = buf[2] - buf[1];
            seq_t g3 = buf[3] - buf[2];
            seq_t g4 = buf[4] - buf[3];

            if (g1 > g2 && g2 > g3 && g3 > g4) {
                /* Ensure cache capacity. */
                if (cache_cnt >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                /* Store the 4th prime of the quintet (index 3). */
                cache[cache_cnt++] = buf[3];
            }
        }
    }

    /* Return the nth term (1‑based index). */
    return cache[(size_t)(n - 1)];
}
