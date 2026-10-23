/*
 * A154676  Numbers n = 103*k^2 such that (n-1,n+1) is a twin prime pair (thus
 *          k = 6*m).
 * https://oeis.org/A154676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   * a(n) = 3708 * m², * where m is the n‑th positive integer such
 *            that 3708*m² − 1 and * 3708*m² + 1 are both prime. (m itself is
 *            not prime in general.)
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
 
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */
 
/* ------------------------------------------------------------------ */
/*  Dynamic cache of computed terms.  The cache grows as needed.        */
static seq_t *cache     = NULL;   /* array of terms */
static idx_t  cache_sz  = 0;      /* number of terms stored       */
static size_t cache_cap = 0;      /* allocated capacity            */
static idx_t  cur_m     = 0;      /* last candidate m examined    */
 
/* ------------------------------------------------------------------ */
/*  Helper to grow the cache when necessary. */
static void ensure_cache_capacity(void)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1024 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }
}
 
/* ------------------------------------------------------------------ */
/*  Main sequence generator.  External linkage as required.           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined offset 0 */
 
    /* Initialize cache on first use. */
    if (!cache) {
        cache = (seq_t *)malloc(1024 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = 1024;
    }
 
    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        cur_m++;                                 /* next candidate m  */
 
        /* Compute 3708 * m² in seq_t.  Cast m to seq_t before
           multiplication to avoid overflow of the 64‑bit intermediate. */
        seq_t val  = (seq_t)3708;
        val *= (seq_t)cur_m;
        val *= (seq_t)cur_m;                     /* val = 3708 * m²  */
 
        seq_t lo = val - 1;                      /* 3708*m² - 1      */
        seq_t hi = val + 1;                      /* 3708*m² + 1      */
 
        if (isPrimeGeneric(lo) && isPrimeGeneric(hi)) {
            ensure_cache_capacity();
            cache[cache_sz++] = val;             /* store the term   */
        }
    }
 
    return cache[n - 1];                         /* 1‑based indexing */
}
