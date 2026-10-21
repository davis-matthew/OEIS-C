/*
 * A214177  Sum of the 4 nearest neighbors of n in a spiral with positive
 *          integers.
 * https://oeis.org/A214177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 5202, u32 n <= 5202, u64 n <= 5202,
 *            u128 n <= 5202, big any n
 * Helpers:   minus_val, plus_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214177: Sum of the 4 nearest neighbors of n in a spiral with positive integers.
   Formula: a(1)=20, a(2)=24, a(3)=32.
   For n >= 3, let d(n) = a(n+1)-a(n). Then
      d(n) = 4  except
        if n = k^2/4 + 3k/2 + (17-(-1)^k)/8  (k>=1) then d(n) = 4k+16
        if n = k^2/4 + 3k/2 + (25-(-1)^k)/8  (k>=0) then d(n) = -(4k+8)   */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/minus_val.h"
#include "../utils/plus_val.h"

/* Cache for computed terms.  All static variables keep state between calls. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_len = 0;     /* number of terms already computed */

/* Current k values for the next special indices. */
static idx_t next_plus_k   = 1;  /* starts with k=1 (first positive special) */
static idx_t next_plus_val = 0;  /* will be set on first initialization */
static idx_t next_minus_k  = 0;  /* starts with k=0 (first negative special) */
static idx_t next_minus_val = 0; /* will be set on first initialization */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defensive; OEIS offset is 1 */

    idx_t target = n - 1;                /* array index */

    /* Allocate space if needed. */
    if (cache_len == 0) {
        /* Initial capacity sufficient for the first three terms. */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */

        /* Base values. */
        cache[0] = (seq_t)20;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)32;
        cache_len = 3;

        /* Initialize special indices. */
        next_plus_val  = plus_val(next_plus_k);   /* k=1 -> n=4 */
        next_minus_val = minus_val(next_minus_k); /* k=0 -> n=3 */
    }

    /* Grow cache if we need to compute more terms. */
    if (cache_len <= target) {
        /* Ensure enough capacity. */
        if (cache_cap <= (size_t)target) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)target) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();       /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from cache_len up to target. */
        for (idx_t i = cache_len; i <= target; ++i) {
            if (i == next_plus_val) {
                /* Positive jump. */
                cache[i] = cache[i-1] + (seq_t)(4*next_plus_k + 16);
                ++next_plus_k;
                next_plus_val = plus_val(next_plus_k);
            } else if (i == next_minus_val) {
                /* Negative jump. */
                cache[i] = cache[i-1] - (seq_t)(4*next_minus_k + 8);
                ++next_minus_k;
                next_minus_val = minus_val(next_minus_k);
            } else {
                /* Regular increment of 4. */
                cache[i] = cache[i-1] + (seq_t)4;
            }
        }
        cache_len = target + 1;
    }

    return cache[target];
}
