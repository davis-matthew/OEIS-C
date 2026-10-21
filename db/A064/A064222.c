/*
 * A064222  a(0) = 0; a(n) = DecimalDigitsSortedDecreasing(a(n-1) + 1) for n >
 *          0.
 * https://oeis.org/A064222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, base, look
 * Formula:   a(n) = DecimalDigitsSortedDecreasing(a(n-1)+1), a(0)=0
 * Range:     u8 n <= 33, u16 n <= 115, u32 n <= 435, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sort_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/sort_desc.h"

static seq_t *cache = NULL;   /* dynamic array of sequence values */
static size_t  cap   = 0;     /* current capacity of cache   */
static idx_t   last  = 0;     /* largest index already computed */

/* ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {                /* first access: initialise a(0) */
        cap  = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        last = 0;
    }
    if (n <= last) return;              /* already computed */

    /* extend capacity if required */
    while (cap <= n) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* out‑of‑memory, terminate */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = prev + (seq_t)1;    /* a(n-1)+1 */
        cache[i] = sort_desc(next);      /* sort digits descending */
    }
    last = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
