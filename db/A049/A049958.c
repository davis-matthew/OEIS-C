/*
 * A049958  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* Recurrence:
   a(1) = 1, a(2) = 2, a(3) = 3
   for n >= 4
     let p = floor(log2(n-1))   (i.e. the unique p such that 2^p < n-1 <= 2^(p+1))
     let m = n-1-2^p
     a(n) = (a(1)+...+a(n-1)) + a(m)   (prefix sums plus a(m)) */
seq_t a(idx_t n) {
    /* Cached values and their prefix sums */
    static seq_t *a_cache   = NULL;   /* a_cache[i] = a(i)  with 0 <= i <= cache_cap-1 */
    static seq_t *pref_cache = NULL;   /* pref_cache[i] = a(1)+...+a(i), pref_cache[0] = 0 */
    static size_t cache_cap = 0;       /* allocated size of the arrays */
    static idx_t  cache_n   = 0;       /* largest index already computed */

    /* Grow arrays if necessary (new_cap is at least n+1) */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;

        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        pref_cache = (seq_t *)realloc(pref_cache, new_cap * sizeof(seq_t));

        if (cache_cap == 0) {
            /* initialise first prefix entry */
            pref_cache[0] = (seq_t)0;
        }
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        if (i == (idx_t)1) {
            a_cache[1] = (seq_t)1;
            pref_cache[1] = a_cache[1];
        } else if (i == (idx_t)2) {
            a_cache[2] = (seq_t)2;
            pref_cache[2] = pref_cache[1] + a_cache[2];
        } else if (i == (idx_t)3) {
            a_cache[3] = (seq_t)3;
            pref_cache[3] = pref_cache[2] + a_cache[3];
        } else {
            /* i >= 4 */
            idx_t x = i - 2; /* i-2 >= 2 */
            /* p = floor(log2(x)) computed via builtin clz */
            idx_t p = 63 - __builtin_clzll((unsigned long long)x);
            idx_t power = ((idx_t)1) << p;   /* 2^p */
            idx_t m = i - 1 - power;         /* n-1-2^p */
            a_cache[i] = pref_cache[i - 1] + a_cache[m];
            pref_cache[i] = pref_cache[i - 1] + a_cache[i];
        }
    }
    cache_n = n;
    return a_cache[n];
}
