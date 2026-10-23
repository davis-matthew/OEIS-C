/*
 * A119826  Number of ternary words of length n with no 000's.
 * https://oeis.org/A119826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=9,
   a(n)=2*(a(n-1)+a(n-2)+a(n-3))  for n>=3   (generate function (1+z+z²)/(1‑2z‑2z²‑2z³) ) */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with base values on first call */
    if (cache_cap == 0) {
        /* indices 0..2 */
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    size_t target = (size_t)n;
    size_t old_cap = cache_cap;

    /* Grow the cache if necessary */
    if (target + 1 > cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc might fail, but we ignore errors to keep the stub minimal */
        if (tmp != NULL) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute missing values up to n */
    for (size_t i = old_cap; i <= target; ++i) {
        /* i is at least 3 here because old_cap is at least 3 after init */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
        cache[i] = sum + sum;           /* multiply by 2 */
    }

    return cache[target];
}
