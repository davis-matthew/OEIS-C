/*
 * A143167  Second column of triangle A000369: |S2(-3;n+2,2)|.
 * https://oeis.org/A143167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (8*n+1)*a(n-1) - 2*(4*n-1)*(2*n-1)*a(n-2)
 * with a(0)=1, a(1)=9
 * This implements the recurrence with memoisation and works for any
 * unsigned integer seq_t (of any width) or for an arbitrary‑precision
 * integer class.  It is valid C11 and C++17.  Intermediate values for
 * built‑in types use unsigned __int128 to avoid overflow.
 */

#include <stdlib.h>

/* cache of computed values */
static seq_t *cache = NULL;
/* capacity of the cache (number of stored elements) */
static idx_t cache_cap = 0;
/* highest index that has been computed; (idx_t)-1 means none yet */
static idx_t last_computed = (idx_t)-1;

/* ensure the cache can hold indices 0 .. n   (n >= last index) */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;          /* already big enough */
    idx_t new_cap = (n + 1) * 2;         /* double to reduce reallocs */
    if (new_cap == 0) new_cap = 2;       /* never zero */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* OOM: terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute all terms up to n, storing them in cache */
static void
compute_to(idx_t n)
{
    /* initialise cache if first call */
    if (last_computed == (idx_t)-1) {
        ensure_capacity(1);              /* at least space for indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        last_computed = 1;
    }

    if (n <= last_computed) return;     /* nothing to do */

    ensure_capacity(n);                 /* make room for index n */

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev1 = cache[i-1];
        seq_t prev2 = cache[i-2];

        /* use 128‑bit intermediate for built‑in types */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 t1 = (unsigned __int128)(8 * i + 1) * (unsigned __int128)prev1;
            unsigned __int128 t2 = (unsigned __int128)2 * (unsigned __int128)(4 * i - 1)
                                   * (unsigned __int128)(2 * i - 1) * (unsigned __int128)prev2;
            cache[i] = (seq_t)(t1 - t2);
        } else {                         /* assume overloaded operators (big integer) */
            seq_t t1 = (seq_t)(8 * i + 1) * prev1;
            seq_t t2 = (seq_t)2 * ((seq_t)(4 * i - 1))
                     * ((seq_t)(2 * i - 1)) * prev2;
            cache[i] = t1 - t2;
        }
    }
    last_computed = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    compute_to(n);
    return cache[n];
}
