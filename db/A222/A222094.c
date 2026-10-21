/*
 * A222094  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          without move-in move-out left turns.
 * https://oeis.org/A222094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 90*a(n-2) + 217*a(n-3) - 258*a(n-4) +
 *            156*a(n-5) - 44*a(n-6) + 4*a(n-7); Initial values: a(1)=3,
 *            a(2)=31, a(3)=306, a(4)=3014, a(5)=29671, a(6)=292017,
 *            a(7)=2873693
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

/* Helper to grow the cache array as needed */
static void ensure_cache(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 8 : *cap * 2;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* fatal on allocation failure */
    *arr = tmp;
    *cap = newcap;
}

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)            /* non‑positive index is undefined */
        return (seq_t)0;

    /* Convert to a native index type for array limits */
    const size_t target = (size_t)n;

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* current capacity of the array */

    /* Grow cache storage if necessary */
    ensure_cache(target, &cache_cap, &cache);

    /* Initial seven terms (hard‑coded, modulo seq_t size) */
    static const seq_t init[7] = {
        (seq_t)3,
        (seq_t)31,
        (seq_t)306,
        (seq_t)3014,
        (seq_t)29671,
        (seq_t)292017,
        (seq_t)2873693
    };

    /* Compute any terms that are not yet in the cache */
    for (size_t i = cache_len; i < target; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            seq_t t1 = (seq_t)17  * cache[i-1];
            seq_t t2 = (seq_t)90  * cache[i-2];
            seq_t t3 = (seq_t)217 * cache[i-3];
            seq_t t4 = (seq_t)258 * cache[i-4];
            seq_t t5 = (seq_t)156 * cache[i-5];
            seq_t t6 = (seq_t)44  * cache[i-6];
            seq_t t7 = (seq_t)4   * cache[i-7];
            cache[i] = t1 - t2 + t3 - t4 + t5 - t6 + t7;
        }
    }
    cache_len = target;

    /* Return the requested term (array indices start at 0 for a(1)) */
    return cache[n-1];
}
