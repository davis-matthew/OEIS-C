/*
 * A174191  Expansion of (1+x)*(2*x-1)/((1-x)*(x^2+2*x-1)).
 * https://oeis.org/A174191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) – 2, a(0)=1, a(1)=2 * (Equivalent to
 *            the recurrence given in the OEIS entry for A174191.)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* Utility functions that allocate and free arrays of seq_t.  They take
 * care of construction/destruction of non‑trivial seq_t types in both C
 * and C++.
 */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cache for previously computed values.  The cache is lazily grown
 * as needed.  All accesses to seq_t in this module use normal
 * arithmetic operators, which are provided for both builtin integer
 * types and for the arbitrary‑precision integer class used in the
 * big‑integer build.
 */
static seq_t *cache = NULL;          /* array of cached terms          */
static size_t cache_cap = 0;         /* allocated capacity (bytes)    */
static idx_t cache_len = 0;          /* number of terms currently stored */

/* Ensure that the cache can store the term with index n.  If a larger
 * capacity is required, a new array is allocated via alloc_seq_array,
 * the old contents are copied over and the old array freed with
 * free_seq_array.  For both the C and C++ builds this strategy preserves
 * the copy/move semantics of seq_t.
 */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;                       /* already large enough */

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap); /* constructs seq_t objects */
    if (!new_cache) abort();

    /* copy old values to the new location */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free the old cache */
    free_seq_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the two starting terms if it is empty. */
static void init_cache(void)
{
    if (cache_len > 0)
        return;

    ensure_cache_cap(1);          /* guarantees capacity for indices 0 and 1 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache_len = 2;
}

/* Public function requested by the build system.  Computes a(n) for
 * any non‑negative n.  The function is memoised so that repeated calls
 * for arbitrary indices are efficient.
 */
seq_t a(idx_t n)
{
    /* The sequence is only defined for non‑negative indices.  In case
     * a caller uses the C convention of casting -1 to an unsigned type
     * we return 0.  This path is never exercised by the OEIS test harness.
     */
    if (n == (idx_t)(-1))
        return (seq_t)0;

    init_cache();

    if (n < cache_len)
        return cache[n];

    ensure_cache_cap(n);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* recurrence: a(i) = 2*a(i-1) + a(i-2) – 2 */
        seq_t val = 2 * cache[i - 1] + cache[i - 2] - (seq_t)2;
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
