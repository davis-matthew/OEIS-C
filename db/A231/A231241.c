/*
 * A231241  Number of n X 3 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A231241
 *  Recurrence (n > 5):
 *      a(n) = 4*a(n-1) + 16*a(n-2) + 26*a(n-3) + 37*a(n-4) + 6*a(n-5)
 *  Initial values:
 *      a(1) = 0,  a(2) = 14,  a(3) = 78,  a(4) = 552,  a(5) = 3820
 *
 *  The function caches computed terms so that later calls are O(1).
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Static cache: cache[i] stores a(i+1). */
static seq_t *cache = NULL;
static idx_t  cached = 0;   /* number of terms stored */

/* Ensure values up to index n (1‑based) are computed and cached. */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;                   /* nothing to do */

    /* Double the capacity (or at least n) to amortise realloc cost. */
    size_t new_cap = (size_t)n * 2;
    if (new_cap == 0)
        new_cap = 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed – terminate the program */
    cache = tmp;

    /* Compute terms from cached+1 up to n. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 1: val = (seq_t)0;        break;
            case 2: val = (seq_t)14;       break;
            case 3: val = (seq_t)78;       break;
            case 4: val = (seq_t)552;      break;
            case 5: val = (seq_t)3820;     break;
            default:
                /* Recurrence – all indices are 1‑based. */
                val  = mul_small(cache[i-2], 4)   /* a(i-1) */
                     + mul_small(cache[i-3], 16)  /* a(i-2) */
                     + mul_small(cache[i-4], 26)  /* a(i-3) */
                     + mul_small(cache[i-5], 37)  /* a(i-4) */
                     + mul_small(cache[i-6], 6);   /* a(i-5) */
                break;
        }
        cache[i-1] = val;              /* store a(i) */
    }
    cached = n;                       /* update cached count */
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, return 0 */
    ensure_cache(n);
    return cache[n-1];                /* cache[0] = a(1) */
}
