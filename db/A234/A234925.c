/*
 * A234925  Primes which become squares when the digits are rotated once to
 *          the right.
 * https://oeis.org/A234925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 30, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Search for primes whose right rotation is a perfect square.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* static storage for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t generated = 0;     /* number of terms already generated */
static seq_t last_candidate = 2;/* next integer to test for primality */

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0, return 0 */

    while (generated < n) {
        if (is_prime_big(last_candidate)) {
            seq_t d = last_candidate % 10;          /* last decimal digit */
            int  len = digit_len(last_candidate);   /* number of digits     */

            /* term after right rotation: d * 10^(len-1) + floor(p/10) */
            seq_t pow10len = pow10((idx_t)(len - 1));
            seq_t rotated = d * pow10len + (last_candidate / 10);

            seq_t sq = floor_sqrt(rotated);
            if (sq * sq == rotated) {               /* rotated is a square */
                if (generated >= cache_cap) {        /* grow cache if needed */
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_ptr = (seq_t *)realloc(cache,
                                                        new_cap * sizeof(seq_t));
                    if (!new_ptr) abort();           /* allocation failed */
                    cache = new_ptr;
                    cache_cap = new_cap;
                }
                cache[generated] = last_candidate;   /* store term */
                ++generated;
            }
        }
        last_candidate = last_candidate + 1; /* check next integer */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}
