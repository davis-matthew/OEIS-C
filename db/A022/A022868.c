/*
 * A022868  a(n) = [ a(n-1)/a(1) ] + [ a(n-1)/a(2) ] + ... + [ a(n-1)/a(n-1) ]
 *          for n >= 3, with initial terms 2,2.
 * https://oeis.org/A022868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n-1} ⌊a(n‑1)/a(i)⌋ for n ≥ 3, with a(1) = 2,
 *            a(2) = 2. This implementation caches results so (a(n)) can be
 *            called in any order
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache (0‑based: a_cache[0] = a(1)). */
static seq_t *a_cache = NULL;
static size_t a_cached = 0;      /* number of elements already stored */
static size_t a_capacity = 0;    /* allocated size of a_cache */

/* Grow cache to hold at least 'needed' elements. */
static void a_grow_cache(size_t needed)
{
    if (a_capacity >= needed) return;
    size_t new_cap = a_capacity ? a_capacity << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);             /* out of memory */
    a_cache = p;
    a_capacity = new_cap;
}

/* Return the n‑th term of OEIS A022868 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard – not used by OEIS */
    if (n <= 2) return (seq_t)2;   /* a(1) = a(2) = 2 */

    /* If the requested term is already cached, return it immediately. */
    if (a_cached >= (size_t)n) return a_cache[n-1];

    /* Ensure the cache is large enough. */
    a_grow_cache((size_t)n);

    /* Initialise first two terms if starting from scratch. */
    if (a_cached == 0) {
        a_cache[0] = (seq_t)2;     /* a(1) */
        a_cache[1] = (seq_t)2;     /* a(2) */
        a_cached = 2;
    }

    /* Compute successive terms up to n. */
    for (idx_t k = a_cached + 1; k <= n; ++k) {
        seq_t prev = a_cache[k-2];   /* a(k‑1) */
        seq_t sum = 0;
        /* Sum floor(prev / a(i)) for i = 1 .. k-1 */
        for (idx_t j = 1; j <= k-1; ++j)
            sum += prev / a_cache[j-1];
        a_cache[k-1] = sum;
        a_cached = k;
    }

    return a_cache[n-1];
}
