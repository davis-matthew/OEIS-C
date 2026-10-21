/*
 * A209842  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          one or two distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A209842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 127*a(n-2) + 90*a(n-3) + 288*a(n-4) -
 *            400*a(n-5) + 128*a(n-6) Initial values: a(1)=41, a(2)=451,
 *            a(3)=5083, a(4)=59563, a(5)=715307, a(6)=8743851
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[6] = {41, 451, 5083, 59563, 715307, 8743851};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* capacity of cache array (including index 0) */
static idx_t cache_last = 0;  /* largest n that has been computed */

/* Ensure cache can hold terms up to n (index 1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_last >= n)
        return;   /* already computed */

    /* Grow the cache to accommodate n (add 1 for unused index 0) */
    idx_t cap_needed = n + 1;
    if (cap_needed > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 7;
        if (new_cap < cap_needed) new_cap = cap_needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first six values if not already */
    if (cache_last < 6) {
        idx_t up = (n < 6 ? n : 6);
        for (idx_t i = 1; i <= up; ++i) cache[i] = init[i - 1];
        if (up > cache_last) cache_last = up;
    }

    /* Compute terms 7 .. n using the recurrence, avoiding underflow */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i - 1];
            continue;
        }
        const seq_t t1 = cache[i - 1];
        const seq_t t2 = cache[i - 2];
        const seq_t t3 = cache[i - 3];
        const seq_t t4 = cache[i - 4];
        const seq_t t5 = cache[i - 5];
        const seq_t t6 = cache[i - 6];

        seq_t pos = 22 * t1;
        pos += 90 * t3;
        pos += 288 * t4;
        pos += 128 * t6;

        seq_t neg = 127 * t2;
        neg += 400 * t5;

        cache[i] = pos - neg;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, but return 0 */
    ensure_cache(n);
    return cache[n];
}
