/*
 * A086512  Rearrangement of natural numbers such that every (2n-1)-th partial
 *          sum is prime. Every 2n-th term is the smallest number not included
 *          earlier.
 * https://oeis.org/A086512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   ensure_used_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A086512:  a(2n) is the smallest unused natural number, and
   every (2n-1)-th partial sum is prime. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/ensure_used_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* value for n<1 is unused */

    /* static data shared across calls */
    static seq_t *cache   = NULL;              /* stored terms */
    static size_t cache_cap = 0, cache_len = 0;
    static unsigned char *used = NULL;         /* flag array of used numbers */
    static size_t used_cap = 0;
    static idx_t sum = 0;                      /* running partial sum */

    /* enlarge cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        idx_t cand = 1;
        for (;;) {
            ensure_used_capacity(cand, &used, &used_cap);
            if (used[(size_t)cand]) {  /* already used */
                ++cand;
                continue;
            }
            if ((i & 1ULL) == 1) {      /* odd index → prime needed */
                idx_t s = sum + cand;
                if (!is_prime(s)) {     /* sum not prime, try next candidate */
                    ++cand;
                    continue;
                }
            }
            /* valid candidate found */
            break;
        }
        cache[i-1] = (seq_t)cand;
        used[(size_t)cand] = 1;
        sum += cand;
    }
    cache_len = n;
    return cache[n-1];
}
