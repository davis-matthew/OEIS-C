/*
 * A084969  Numbers whose smallest prime factor is 11.
 * https://oeis.org/A084969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A084969: Numbers whose smallest prime factor is 11.  
   The sequence is 11 times the numbers coprime to 210 (2·3·5·7).  
   Recurrence for n > 48:  a(n) = a(n-48) + 2310  
   Initial values: a(1..48) are 11·k for 1 ≤ k ≤ 210 with gcd(k,210)=1. 
*/

#include <stdlib.h>
#include "../utils/gcd_small.h"

/* static cache for results */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms actually stored */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* ensure cache contains at least n terms, allocating as necessary */
static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;

    /* allocate or enlarge storage */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise first 48 terms if cache is empty */
    if (cache_size == 0) {
        const idx_t M = 210;               /* product of 2·3·5·7 */
        const seq_t mult = 11;              /* multiplier for the result */
        idx_t count = 0;
        for (idx_t k = 1; k <= M && count < 48; ++k) {
            if (gcd_small(k, M) == 1) {
                cache[cache_size++] = (seq_t)mult * (seq_t)k;
                ++count;
            }
        }
    }

    /* extend cache using recurrence for n > 48 */
    while (cache_size < n) {
        seq_t val = cache[cache_size - 48] + (seq_t)2310; /* 11 * 210 */
        cache[cache_size++] = val;
    }
}

/* public entry point */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not used – offsets start at 1 */
    ensure_cache(n);
    return cache[n - 1];
}
