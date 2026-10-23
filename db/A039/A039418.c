/*
 * A039418  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 5's.
 * https://oeis.org/A039418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   equal_3_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: the nth positive integer whose decimal expansion
   contains the same non‑zero number of digits 3 and 5.  
   The function computes the term by brute‑force searching and
   caching results. */

/*------------------------------------*/
/*  Standard headers used             */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_3_5.h"

/*------------------------------------*/
/*  State for memoisation             */
static seq_t *cache = NULL;      /* array of found terms            */
static size_t cache_cap = 0;     /* current allocated capacity      */
static size_t cache_len = 0;     /* number of terms stored          */
static seq_t last_checked = 0;   /* last integer examined           */

/*------------------------------------*/

/*------------------------------------*/
/*  Ensure cache can hold at least
   'needed' entries.                     */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the test harness an allocation failure is not expected. */
    cache = tmp;
    cache_cap = new_cap;
}

/*------------------------------------*/
/*  The requested sequence function.   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* off‑by‑offset; undefined for n=0 */

    size_t target = (size_t)n;
    if (target <= cache_len) return cache[n-1];

    grow_cache(target);

    /* Scan integers until we have enough terms. */
    while (cache_len < target)
    {
        last_checked += (seq_t)1;
        if (equal_3_5(last_checked))
            cache[cache_len++] = last_checked;
    }

    return cache[n-1];
}
