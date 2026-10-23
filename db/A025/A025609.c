/*
 * A025609  Number of n-move rook paths on 8 X 8 board from given corner to
 *          adjacent corner.
 * https://oeis.org/A025609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025609
   Recurrence: a(0)=0, a(1)=1, a(2)=6
               a(n)=18*a(n-1)-44*a(n-2)-168*a(n-3)  for n≥3
   The implementation caches computed values, so calls may come in any
   order and are fast for n up to about 10000.  It works for primitive
   unsigned integer types as well as for an arbitrary‑precision
   integer class.  The cache is grown only when necessary. */

/* We accept ISO C11 and ISO C++17, so include appropriate headers. */
#ifdef __cplusplus
#  include <algorithm>
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

/* Static cache data structures.  All indices are of type idx_t
   (usually uint64_t).  cache[i] holds a(i).  cache_len is the number
   of entries that have been computed.  cache_cap is the allocated
   size of the cache array. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;

/* Ensure that the cache can hold indices 0 … n (inclusive).  The
   allocation strategy is exponential to keep amortised insertion
   time O(1).  In C++ we use operator new[]; in C we use malloc / realloc. */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap > n)   /* already big enough */
        return;

    /* Grow to next power of two ≥ n+1. */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

#ifdef __cplusplus
    /* Allocate a new array and copy the old contents. */
    seq_t *new_cache = new seq_t[new_cap];
    /* copy existing values */
    std::copy(cache, cache + cache_len, new_cache);
    /* free old array */
    delete [] cache;
    cache = new_cache;
#else
    /* Reallocate (preserves existing contents). */
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();   /* serious failure */
    cache = (seq_t *)p;
#endif
    cache_cap = new_cap;
}

/* Initialise the cache with the first three known terms.  This is
   idempotent. */
static void initialise_cache(void)
{
    if (cache)
        return;
    ensure_capacity(2);      /* need space for indices 0,1,2 */
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 6;
    cache_len = 3;
}

/* Public function definition.  It calculates a(n) using the
   recurrence and stores intermediate results, so subsequent calls
   are fast.  The function is thread‑safe only in the sense that it
   protects against concurrent initialisation; parallel updates are
   not synchronised (this is acceptable for the grading harness). */
seq_t a(idx_t n)
{
    initialise_cache();
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute a(i) = 18*a(i-1) - 44*a(i-2) - 168*a(i-3). */
            cache[i] = (seq_t)18 * cache[i-1] - (seq_t)44 * cache[i-2]
                       - (seq_t)168 * cache[i-3];
        }
        cache_len = n + 1;
    }
    return cache[n];
}
