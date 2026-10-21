/*
 * A076205  Numbers n such that 30*n+{1,7,11,13,17,19,23,29} are all
 *          composite.
 * https://oeis.org/A076205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4042, u32 n <= 6829, u64 n <= 6829,
 *            u128 n <= 6829, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for n such that 30*n+{1,7,11,13,17,19,23,29} are all composite */
/* The function builds the sequence lazily and caches results. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Residues modulo 30 that must all be composite at 30*n + residue */
    static const int residues[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    /* Static cache of found terms, expanding as needed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* current allocated size          */
    static size_t found = 0;    /* number of terms stored in cache  */
    static idx_t next_candidate = 1;   /* next n to test     */

    /* Expand cache to accommodate at least 'n' terms when necessary */
    if (found < n) {
        if (cap < n) {
            size_t new_cap = (cap == 0) ? 64 : cap;
            while (new_cap < n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
            cap = new_cap;
        }

        /* Search until we have at least 'n' terms */
        while (found < n) {
            bool good = true;
            for (int i = 0; i < 8 && good; ++i) {
                idx_t m = 30ULL * next_candidate + residues[i];
                if (is_prime(m))
                    good = false;
            }
            if (good) {
                cache[found++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
