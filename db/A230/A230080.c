/*
 * A230080  Sequence needed for the nonpositive powers of rho(11) =
 *          2*cos(Pi/11) in terms of the power basis of the degree 5 number
 *          field Q(rho(11)). Coefficients of the first power.
 * https://oeis.org/A230080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5) (n ≥ 5)
 *            Initial terms: a(0)=0, a(1)=3, a(2)=5, a(3)=23, a(4)=73
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Static cache: a[0] .. a[cache_len-1] */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    const idx_t old_len = cache_len;
    const idx_t new_len = (n + 1 > 5) ? (n + 1) : 5;   /* at least five slots */

    seq_t *new_cache = alloc_seq_array(new_len);
    if (!new_cache)
        exit(EXIT_FAILURE);

    /* Copy already computed terms */
    for (idx_t i = 0; i < old_len; ++i)
        new_cache[i] = cache[i];

    /* Initialise the five first terms if this is the first call */
    if (old_len == 0) {
        new_cache[0] = 0;
        new_cache[1] = 3;
        new_cache[2] = 5;
        new_cache[3] = 23;
        new_cache[4] = 73;
    }

    /* Compute any missing terms */
    idx_t start = (old_len > 5) ? old_len : 5;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val  =  3 * new_cache[i - 1];
        val       +=  3 * new_cache[i - 2];
        val       -=  4 * new_cache[i - 3];
        val       -=  new_cache[i - 4];
        val       +=  new_cache[i - 5];
        new_cache[i] = val;
    }

    /* Release the old cache and record the new one */
    if (cache)
        free_seq_array(cache);
    cache      = new_cache;
    cache_len  = new_len;
}

/* Return a(n) for any non‑negative n.  The function works regardless of
   the order of calls because it keeps a static cache of previously
   computed terms. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
