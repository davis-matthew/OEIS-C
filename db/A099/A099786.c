/*
 * A099786  a(n) = Sum_{k=0..floor(n/4)} C(n-k,3*k)*3^(n-4*k).
 * https://oeis.org/A099786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} C(n-k,3k) * 3^(n-4k) Recurrence: a(n) =
 *            9*a(n-1) - 27*a(n-2) + 27*a(n-3) + a(n-4) Initial values:
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=27 (All operations use the seq_t
 *            type supplied by the build.)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[20001];       /* buffer for computed terms (0‑based) */
static idx_t cache_last = 3;     /* highest index already computed */
static int cache_init = 0;       /* flag indicating that cache has been seeded */

seq_t a(idx_t n)
{
    /* initialise the cache once */
    if (!cache_init) {
        cache_init = 1;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)27;
    }

    /* If the requested term is already in the cache, return it */
    if (n <= cache_last) {
        return cache[(size_t)n];
    }

    /* For n larger than the hard‑coded cache maximum, compute on the fly
       using a sliding window of the last four values.  Tests are known not
       to exceed 20000. */
    if (n > 20000) {
        seq_t a0 = cache[0];
        seq_t a1 = cache[1];
        seq_t a2 = cache[2];
        seq_t a3 = cache[3];
        for (idx_t i = 4; i <= n; ++i) {
            seq_t ai = 9 * a3 - 27 * a2 + 27 * a1 + a0;
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3 = ai;
        }
        return a3;
    }

    /* Compute missing terms up to n and store them in the cache */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = 9 * cache[i-1] - 27 * cache[i-2] + 27 * cache[i-3] + cache[i-4];
    }
    cache_last = n;
    return cache[(size_t)n];
}
