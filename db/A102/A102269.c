/*
 * A102269  Primes p such that Q(sqrt(-21p)) has genus characters chi_{-3} =
 *          +1, chi_{-7} = +1.
 * https://oeis.org/A102269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that p ≡ 43, 67 or 79 (mod 84)
   (equivalently p ≡ 3 (mod 4) with (−3/p) = (−7/p) = 1).
   The function caches results so that arbitrary calls are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#define RESIDUES_COUNT 3
static const unsigned long RESIDUES[RESIDUES_COUNT] = {43, 67, 79};

seq_t a(idx_t n) {
    /* Handle out-of-range indices gracefully */
    if (n == 0) return (seq_t)0;  /* undefined, return 0 */

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static idx_t cache_capacity = 0; /* allocated size */
    static idx_t current_k = 0;    /* counter for multiples of 84 */
    static int residue_index = 0;  /* index into RESIDUES */

    /* Ensure enough space in cache */
    if (n > cache_capacity) {
        idx_t new_capacity = (cache_capacity == 0) ? 256 : cache_capacity * 2;
        while (new_capacity < n) new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) { /* allocation failed; return 0 as sentinel */
            free(cache);
            cache = NULL;
            cache_size = cache_capacity = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Generate terms until we have at least n terms */
    while (cache_size < n) {
        unsigned long candidate = 84u * current_k + RESIDUES[residue_index];
        if (candidate >= 3u && is_prime(candidate)) {
            cache[cache_size++] = (seq_t)candidate;
        }
        /* Advance to next residue / k */
        residue_index++;
        if (residue_index == RESIDUES_COUNT) {
            residue_index = 0;
            current_k++;
        }
    }

    return cache[n - 1];
}
