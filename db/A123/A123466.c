/*
 * A123466  Write the positive integer n in binary. Subdivide the binary n
 *          into runs each consisting entirely of 0's or of 1's, where the
 *          runs alternate between those of 1's and those of 0's. The sequence
 *          gives those numbers n such that there is at least one run of 1's
 *          of the same length as at least one run...
 * https://oeis.org/A123466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates a(n) by searching forward for the nth number whose binary
   representation contains a run of 1's equal to a run of 0's. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_run.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, sequence is defined for n ≥ 1 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */
    static idx_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t last_checked = 1;  /* largest integer examined so far */

    /* grow cache until it contains at least n terms */
    while (cache_size < n) {
        /* find next integer satisfying the property */
        uint64_t val = last_checked + 1;
        while (!has_equal_run(val)) ++val;

        /* ensure enough space */
        if (cache_size >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = (seq_t)val;
        last_checked = val;
    }

    return cache[n - 1];
}
