/*
 * A211463  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having three or four distinct values for every i<=n and j<=n.
 * https://oeis.org/A211463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=20, a(2)=80, a(3)=304, a(4)=1144, a(5)=4232, a(6)=15536,
   a(7)=56616, a(8)=205400
   a(n) = 5*a(n-1)+9*a(n-2)-57*a(n-3)-36*a(n-4)
          +196*a(n-5)+96*a(n-6)-124*a(n-7)+24*a(n-8)   for n ≥ 9
   This implementation memoises all previously computed values, so
   calls can arrive in any order.  It uses unsigned arithmetic only,
   ensuring the intermediate computation never underflows: the
   positive terms are summed first into `p` and the negative terms
   into `n`; the result is `p - n`.  All operations are performed
   with the type seq_t, which may be an arbitrary‑precision
   integer class.  The function is compatible with C11 and C++17.
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;            /* storage for terms, cache[0] unused */
static size_t cache_capacity = 0;      /* number of allocated slots           */
static size_t cache_end = 0;           /* index of first uncached element     */

/* Grow the cache array so that it can hold indices 0 … need-1.  */
static void ensure_cache_capacity(size_t need)
{
    if (cache_capacity >= need) return;
    size_t new_cap = cache_capacity ? cache_capacity : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) exit(EXIT_FAILURE);    /* out of memory */
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Initialise the cache with the first eight terms.  */
static void init_cache(void)
{
    static const seq_t init_vals[9] = {
        0,          /* dummy for index 0 */
        20,         /* a(1) */
        80,         /* a(2) */
        304,        /* a(3) */
        1144,       /* a(4) */
        4232,       /* a(5) */
        15536,      /* a(6) */
        56616,      /* a(7) */
        205400      /* a(8) */
    };
    /* Allocate space for at least indices 0 … 8.  */
    ensure_cache_capacity(9);
    for (size_t i = 0; i <= 8; ++i) cache[i] = init_vals[i];
    cache_end = 9;   /* next index to compute */
}

/* Public entry point requested by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* harmless, not used */

    if (cache == NULL) init_cache();

    /* Make sure the array can hold indices up to n. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute missing terms up to n.  */
    if ((size_t)n + 1 > cache_end) {
        for (size_t i = cache_end; i <= (size_t)n; ++i) {
            seq_t p = (seq_t)5   * cache[i-1] +
                      (seq_t)9   * cache[i-2] +
                      (seq_t)196 * cache[i-5] +
                      (seq_t)96  * cache[i-6] +
                      (seq_t)24  * cache[i-8];
            seq_t sub = (seq_t)57  * cache[i-3] +
                       (seq_t)36  * cache[i-4] +
                       (seq_t)124 * cache[i-7];
            cache[i] = p - sub;   /* p ≥ sub for all n ≥ 9 */
        }
        cache_end = (size_t)n + 1;
    }
    return cache[n];
}
