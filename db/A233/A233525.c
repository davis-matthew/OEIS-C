/*
 * A233525  Start with a(1) = 1, a(2) = 1, then a(n)*3^k = a(n+1) + a(n+2),
 *          with 3^k the smallest power of 3 (k>0) such that all terms a(n)
 *          are positive integers.
 * https://oeis.org/A233525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1. For n ≥ 1, a(n)*3^k = a(n+1) + a(n+2), where
 *            k > 0 is the smallest integer such that a(n)*3^k > a(n+1). The
 *            recurrence uses the minimal power of 3 that keeps all terms
 *            positive integers
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 51, u64 n <= 109,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;         /* memoisation array   */
static size_t cache_len = 0;        /* number of computed terms */
static size_t cache_cap = 0;        /* capacity of cache    */

/* enlarge cache if needed */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap : 4;
    while (newcap < (size_t)n) newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cache_cap = newcap;
}

/* Compute a(n) with memoisation; works for any call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */
    if (n <= 2) return (seq_t)1;

    /* ensure cache contains at least n terms */
    if (cache_len < (size_t)n) {
        ensure_cache_size(n);
        /* initialise first two terms if not yet done */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache_len = 2;
        }
        /* compute missing terms in order */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t a_prev = cache[i - 2];   /* a(n)   */
            seq_t a_next = cache[i - 1];   /* a(n+1) */
            seq_t pow = (seq_t)3;          /* start with 3^1 */
            /* find smallest k such that a_prev * pow > a_next */
            while (a_prev <= a_next / pow) {
                pow *= (seq_t)3;
            }
            seq_t new_a = a_prev * pow - a_next; /* a(n+2) */
            cache[i] = new_a;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
