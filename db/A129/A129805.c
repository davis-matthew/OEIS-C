/*
 * A129805  Primes congruent to +-1 mod 18.
 * https://oeis.org/A129805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ ±1 (mod 9)
 * Range:     u8 n <= 17, u16 n <= 2172, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9836 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (= primes congruent to 1 or 8 modulo 9).  The first few values are */
/* 17, 19, 37, 53, 71, 73, 89, 107, 109, 127, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* array of qualifying primes */
static size_t cache_size = 0;        /* number of stored terms          */
static size_t cache_cap = 0;         /* allocated capacity              */
static uint64_t last_checked = 2;    /* last integer examined (prime or not) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n = 0 */

    /* Extend cache until it contains at least n terms. */
    if (n > cache_size) {
        while (cache_size < (size_t)n) {
            uint64_t cand = last_checked + 1;

            /* Find the next prime after last_checked. */
            while (!is_prime_seq(cand))
                ++cand;

            /* Store primes congruent to ±1 mod 9. */
            uint64_t r = cand % 9;
            if (r == 1 || r == 8) {
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) abort();          /* allocation failure */
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)cand;
            }

            last_checked = cand;             /* move the start point forward */
        }
    }

    return cache[(size_t)n - 1];
}
