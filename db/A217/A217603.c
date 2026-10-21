/*
 * A217603  Consider sets of 3 consecutive primes a, b, c such that c - a =
 *          100, then sequence gives the values of b.
 * https://oeis.org/A217603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the middle prime of the n‑th consecutive triple of primes
 *            (a,b,c) with c-a = 100. The function searches primes in order
 *            until the n‑th such triple is found
 * Range:     u8 none, u16 n <= 1, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   init_small_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/init_small_primes.h"

seq_t a(idx_t n)
{
    /* Ensure the small prime table exists. */
    static uint32_t *small_primes = NULL;
    static size_t   small_cnt    = 0;
    if (!small_primes)
        init_small_primes(&small_primes, &small_cnt);

    uint64_t target   = (uint64_t)n;     /* how many triples we need */
    uint64_t count    = 0;               /* found triples so far */
    uint64_t a_sym    = 0, b_sym = 0;    /* sliding window primes */

    const uint64_t BLOCK = 1u << 20;      /* 1 048 576 numbers per block */
    uint64_t low = 2, high;

    while (count < target) {
        high = low + BLOCK;
        unsigned char *is_comp = (unsigned char *)calloc(BLOCK, 1);
        if (!is_comp) exit(1);

        /* Mark composites using the small primes. */
        for (size_t i = 0; i < small_cnt; ++i) {
            uint32_t p = small_primes[i];
            uint64_t start = (low + p - 1) / p * p;
            uint64_t p2     = (uint64_t)p * p;
            if (start < p2) start = p2;
            if (start >= high) continue;
            for (uint64_t j = start; j < high; j += p)
                is_comp[j - low] = 1;
        }

        /* Scan for primes in the current block. */
        for (uint64_t offset = 0; offset < BLOCK; ++offset) {
            uint64_t val = low + offset;
            if (val < 2) continue;
            if (is_comp[offset]) continue;     /* composite */

            /* val is prime. */
            if (!a_sym) { a_sym = val; continue; }
            if (!b_sym) { b_sym = val; continue; }

            /* val is the candidate c. */
            if (val - a_sym == 100) {
                ++count;
                if (count == target) {
                    free(is_comp);
                    return (seq_t)b_sym;
                }
            }
            /* slide window */
            a_sym = b_sym;
            b_sym = val;
        }

        free(is_comp);
        low = high;      /* next block */
    }

    /* This point is unreachable for n >= 1. */
    return (seq_t)0;
}
