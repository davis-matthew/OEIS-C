/*
 * A043724  Numbers whose number of runs in the base-2 representation is
 *          congruent to 0 mod 4.
 * https://oeis.org/A043724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the n-th positive integer whose binary run
   count (number of maximal consecutive identical bits) is
   congruent to 0 modulo 4.  The sequence starts at a(1)=10. */

#include <stdlib.h>
#include <string.h>

static int count_runs(seq_t x) {
    int runs = 1;                    /* at least one run */
    seq_t prev_bit = x & 1;          /* LSB */
    x >>= 1;
    while (x) {
        seq_t bit = x & 1;
        if (bit != prev_bit) {
            runs++;
            prev_bit = bit;
        }
        x >>= 1;
    }
    return runs;
}

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t stored = 0;        /* number of terms stored (a(1) stored at cache[0]) */
    static seq_t next_candidate = 1;/* next integer to test */

    /* grow cache to accommodate new terms */
    if (n == 0) return 0;           /* offset is 1, but guard against n=0 */
    while (stored < n) {
        if (count_runs(next_candidate) % 4 == 0) {
            if (stored >= cap) {
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *newptr = (seq_t*)malloc(newcap * sizeof(seq_t));
                if (cache) {
                    memcpy(newptr, cache, stored * sizeof(seq_t));
                    free(cache);
                }
                cache = newptr;
                cap = newcap;
            }
            cache[stored] = next_candidate;
            stored++;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
