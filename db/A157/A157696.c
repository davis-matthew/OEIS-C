/*
 * A157696  Define k(n) to be the sequence of integers such that
 *          k(n)F(n)=F(2n)(Fibonacci sequence) (A000204); in turn define g(n)
 *          to be the sequence of integers such that g(n)k(n)=k(3n)(A110391);
 *          finally a(n) is the sequence of integers such that a(n)g(n)=g(5n).
 * https://oeis.org/A157696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A157696.
 *  a(n) = A110391(5*n)/A110391(n)
 *  Recurrence for n ≥ 8:
 *    a(n) = 27*a(n-1) + 904*a(n-2) + 1660*a(n-3)
 *           -1660*a(n-4) - 904*a(n-5) - 27*a(n-6) + a(n-7)
 *  Initial values:
 *    a(1)=31, a(2)=2521, a(3)=97921, a(4)=4974481,
 *    a(5)=226965751, a(6)=10783342081, a(7)=504420084871
 */

#include <stdlib.h>

static seq_t *cache   = NULL;  /* memoisation table, index 0 unused */
static idx_t  cache_sz = 0;    /* largest index already stored */

/* Fixed initial values (index 0 is a dummy). */
static const seq_t init_vals[8] = {
    (seq_t)0,
    (seq_t)31,
    (seq_t)2521,
    (seq_t)97921,
    (seq_t)4974481,
    (seq_t)226965751,
    (seq_t)10783342081ULL,
    (seq_t)504420084871ULL
};

/* Ensure that cache contains a(1)…a(n). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_sz) return;          /* already computed */

    /* Grow the cache to hold indices up to n (including 0). */
    size_t new_sz = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();                  /* allocation failure */
    cache = tmp;

    /* Compute missing terms.  Start from the next unfilled index. */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        if (i <= (idx_t)7) {
            cache[i] = init_vals[(size_t)i];
        } else {
            /* Apply the recurrence.  All referenced terms are already
             * available because i-7 < i. */
            cache[i] =
                cache[i-1] * 27        + cache[i-2] * 904
              + cache[i-3] * 1660       - cache[i-4] * 1660
              - cache[i-5] * 904        - cache[i-6] * 27
              + cache[i-7];
        }
    }
    cache_sz = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* sequence undefined below offset */
    ensure_cache(n);
    return cache[n];
}
