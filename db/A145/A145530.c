/*
 * A145530  Numbers x such that there exists n in N with (x+1)^3-x^3=91*n^2.
 * https://oeis.org/A145530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145530: 
   a(1)=5, a(2)=8002,
   a(n)=1454*a(n-1)-a(n-2)+726  for n≥3.
   Uses memoisation – cache grows to the largest index requested.
*/

#include <stdint.h>
#include <stdlib.h>
/* Big‑integer utilities (work for both C and C++ builds). */
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache   = NULL;   /* array of computed terms                   */
static idx_t  cache_cap = 0;    /* current capacity of `cache` (1‑based size) */
static idx_t  cache_max = 0;    /* largest index stored (1‑based)             */

/* Extend the cache so that it contains a(1)…a(n). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;                 /* already cached         */

    /* ------------------------------------------------------------------ */
    /* (1) Allocate (or re‑allocate) the underlying array                */
    /* ------------------------------------------------------------------ */
    if (cache == NULL) {                         /* first allocation      */
        idx_t new_cap = n;
        if (new_cap < 2) new_cap = 2;            /* need capacity for a(2)*/
        cache = seq_alloc_array(new_cap);
        cache_cap = new_cap;
    } else if (n > cache_cap) {                 /* grow the array        */
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (idx_t i = 0; i < cache_max; ++i)   /* copy existing terms   */
            new_cache[i] = cache[i];
        free_seq_array(cache);                  /* proper destructors     */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* (2) Initialise the first two terms if they are not yet stored.     */
    /* ------------------------------------------------------------------ */
    cache[0] = (seq_t)5;                       /* a(1)                  */
    if (cache_cap >= 2)
        cache[1] = (seq_t)8002;                /* a(2)                  */
    cache_max = 2;                             /* we now have first two  */

    /* ------------------------------------------------------------------ */
    /* (3) Compute the remaining terms up to a(n).                        */
    /* ------------------------------------------------------------------ */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 2];           /* a(i‑1)                 */
        seq_t prev2  = cache[i - 3];           /* a(i‑2)                 */
        cache[i - 1] = (seq_t)1454 * prev - prev2 + (seq_t)726;
    }
    cache_max = n;
}

/* ---------------------------------------------------------------------- */
/*  Public entry point – returns a(n) for n ≥ 1.                           */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* out of range          */
    ensure_cache(n);
    return cache[n - 1];
}
