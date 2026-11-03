/*
 * A145479  Primes p such that (31+p)/2 is prime.
 * https://oeis.org/A145479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute primes p such that (31 + p) / 2 is also prime. * a(1) =
 *            3, a(2) = 7, a(3) = 31, ... * Implemented by generating primes
 *            incrementally and caching the * sequence to support arbitrary
 *            call order
 * Range:     u8 n <= 8, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8306 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

#define CACHE_CAP 20000   /* sufficient for n up to ~10 000 */

/* static cache to store the sequence values */
static seq_t *cache = NULL;   /* array of seq_t */
static idx_t generated = 0;   /* how many terms have been generated */
static uint64_t cand = 3;     /* next odd candidate to test */

/* ensure the cache is initialized once */
static void init_cache(void)
{
    if (!cache) {
        cache = seq_alloc_array(CACHE_CAP);
    }
}

/* generate terms until we have at least n terms */
static void generate_until(idx_t n)
{
    init_cache();

    while (generated < n) {
        /* find the next prime p */
        while (!is_prime(cand)) {
            cand += 2;          /* only odd candidates >2 */
        }

        /* check the secondary condition */
        uint64_t q = (31 + cand) / 2;
        if (is_prime(q)) {
            /* store the term */
            if (generated < CACHE_CAP) {
                cache[generated++] = (seq_t)cand;
            }
        }
        cand += 2;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, return 0 */
    generate_until(n);
    return cache[n - 1];
}
