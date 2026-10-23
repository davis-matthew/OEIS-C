/*
 * A115923  Numbers k such that k and 3*k, taken together, are pandigital.
 * https://oeis.org/A115923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   pandigital5, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115923: Numbers k such that k and 3*k, taken together, are pandigital */
/* The sequence consists of all 5‑digit integers k with 3334 ≤ k ≤ 33333 such that the
   digits of k and 3*k together contain each decimal digit 0–9 exactly once.
   This property is checked by brute force over the 30‑000 candidates. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/pandigital5.h"

static seq_t *cache = NULL;          /* array of found numbers */
static size_t cache_len = 0;          /* number of elements stored   */
static bool cache_built = false;     /* cache initialized?          */

/* Build the cache of all terms on first use. */
static void build_cache(void)
{
    if (cache_built) return;

    const idx_t start = 3334;
    const idx_t end   = 33333;

    /* Count the number of solutions */
    size_t cnt = 0;
    for (idx_t k = start; k <= end; ++k)
        if (pandigital5(k)) ++cnt;

    /* Allocate the cache array */
    cache = seq_alloc_array(cnt);
    if (!cache) { cache_built = true; return; }

    /* Store the solutions */
    size_t pos = 0;
    for (idx_t k = start; k <= end; ++k)
        if (pandigital5(k))
            cache[pos++] = (seq_t)k;

    cache_len   = cnt;
    cache_built = true;
}

/* Return the n‑th term (1‑based) of the sequence, or 0 if n is out of range. */
seq_t a(idx_t n)
{
    build_cache();
    if (n == 0 || n > (idx_t)cache_len) return (seq_t)0;
    return cache[(size_t)n - 1];
}
