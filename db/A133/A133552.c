/*
 * A133552  Number of length n binary sequences with at most 4 of every
 *          adjacent 6 bits set.
 * https://oeis.org/A133552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-5)+2*a(n-6)
 *            -2*a(n-8)-2*a(n-9)-a(n-12)+a(n-15) with initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=31, a(6)=57, a(7)=109,
 *            a(8)=209, a(9)=401, a(10)=769, a(11)=1473, a(12)=2817,
 *            a(13)=5391, a(14)=10321
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initial values */
    static const seq_t init_vals[15] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
        (seq_t)31, (seq_t)57, (seq_t)109, (seq_t)209, (seq_t)401,
        (seq_t)769, (seq_t)1473, (seq_t)2817, (seq_t)5391, (seq_t)10321
    };

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        cache_len = 15;
    }

    if (n < cache_len) return cache[n];

    /* grow capacity if needed */
    while (cache_cap <= n) {
        size_t new_cap = (cache_cap == 0) ? 16 : (size_t)cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0; /* allocation failure */
        cache = new_ptr;
        cache_cap = (idx_t)new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init_vals[i];
            continue;
        }
        seq_t val = 0;
        val += cache[i - 1];
        val += cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 5];
        val += cache[i - 6];
        val += cache[i - 6];          /* 2*a(i-6) */
        val -= cache[i - 8];
        val -= cache[i - 8];          /* 2*a(i-8) */
        val -= cache[i - 9];
        val -= cache[i - 9];          /* 2*a(i-9) */
        val -= cache[i -12];          /* a(i-12) */
        val += cache[i -15];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
