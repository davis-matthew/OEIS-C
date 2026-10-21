/*
 * A214102  Number of 0..2 colorings of a 3X(n+1) array circular in the n+1
 *          direction with new values 0..2 introduced in row major order.
 * https://oeis.org/A214102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) - 33*a(n-3) - 22*a(n-4) + 38*a(n-5)
 *            + 8*a(n-6) - 8*a(n-7) Initial terms: a(1)=9, a(2)=4, a(3)=121,
 *            a(4)=180, a(5)=2041, a(6)=5068, a(7)=37441
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment. */
seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n < 1) return 0;

    /* Initial seven terms (index 0 unused) */
    static const seq_t init[8] = {0, 9, 4, 121, 180, 2041, 5068, 37441};

    if (n <= 7) return init[n];

    /* Cache to store up to the requested n.  Static so that
       subsequent calls reuse the values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0; /* Highest index computed so far */

    /* Allocate and initialise if this is the first call */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= 7; ++i) cache[i] = init[i];
        cache_size = 7;
    }

    /* Grow the buffer if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Positive part */
        seq_t pre = (seq_t)3 * cache[i - 1];
        pre += (seq_t)15 * cache[i - 2];
        pre += (seq_t)38 * cache[i - 5];
        pre += (seq_t)8  * cache[i - 6];

        /* Negative part */
        seq_t sub = (seq_t)33 * cache[i - 3];
        sub += (seq_t)22 * cache[i - 4];
        sub += (seq_t)8  * cache[i - 7];

        /* Result is guaranteed to be non‑negative */
        cache[i] = pre - sub;
    }
    cache_size = n;

    return cache[n];
}
