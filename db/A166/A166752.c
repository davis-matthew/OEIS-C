/*
 * A166752  Interleave A007583 and A000012.
 * https://oeis.org/A166752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(2)=3; a(n)=5*a(n-2)-4*a(n-4) for n≥4 even, a(n)=1 for
 *            n odd
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are defined by the build system; seq_t may be an
 * arbitrary‑precision type (e.g. a C++ class).  All arithmetic is performed
 * using the operators provided by seq_t.  The routine below keeps a cache
 * of the even‑index terms, using a recurrence that requires only the two
 * previous even terms.
 */

#include <stdlib.h>

static seq_t *even_cache = NULL;  /* Cache for a(2k), k ≥ 0 */
static idx_t cached_max_k = 0;    /* Highest k computed so far */
static size_t allocated_cap = 0;  /* Allocated size of even_cache */

/* Ensure that even_cache[0..k] are computed and stored. */
static void ensure_even_cache(idx_t k)
{
    if (even_cache == NULL) {
        /* Allocate space for k=0 and 1 */
        allocated_cap = 4;
        even_cache = (seq_t *)malloc(allocated_cap * sizeof(seq_t));
        if (even_cache == NULL) abort();
        even_cache[0] = (seq_t)1;   /* a(0) */
        even_cache[1] = (seq_t)3;   /* a(2) */
        cached_max_k = 1;
        return;
    }
    if (k <= cached_max_k) return;   /* nothing to do */

    /* Grow capacity when needed */
    if ((size_t)(k + 1) > allocated_cap) {
        size_t newcap = allocated_cap;
        while (newcap <= (size_t)(k + 1)) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(even_cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        even_cache = tmp;
        allocated_cap = newcap;
    }

    /* Compute new terms using the recurrence a(2k) = 5*a(2(k-1)) - 4*a(2(k-2)) */
    for (idx_t i = cached_max_k + 1; i <= k; ++i) {
        even_cache[i] = 5 * even_cache[i - 1] - 4 * even_cache[i - 2];
    }
    cached_max_k = k;
}

seq_t a(idx_t n)
{
    if (n & 1ULL) {          /* odd index → term is 1 */
        return (seq_t)1;
    }
    idx_t k = n >> 1;        /* even index: k = n/2 */
    ensure_even_cache(k);
    return even_cache[k];
}
