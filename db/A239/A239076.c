/*
 * A239076  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 4: |p(i)-i| <= 4.
 * https://oeis.org/A239076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence of order 16 a(n) = a(n-1) +
 *            2*a(n-2) + a(n-3) + a(n-4) + 6*a(n-5) + 9*a(n-6) + 5*a(n-7) +
 *            4*a(n-8) - 3*a(n-9) - a(n-10) - 2*a(n-11) - a(n-12) - a(n-13) -
 *            2*a(n-14) - a(n-15) - a(n-16) with initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=10, a(5)=26, a(6)=...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* first call: allocate and initialize cache with the first 16 terms */
    if (cache_len == 0) {
        cache_len = 16;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* fallback if allocation fails */

        /* initial terms a(0) .. a(15) */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)2;
        cache[3]  = (seq_t)4;
        cache[4]  = (seq_t)10;
        cache[5]  = (seq_t)26;
        cache[6]  = (seq_t)66;
        cache[7]  = (seq_t)158;
        cache[8]  = (seq_t)376;
        cache[9]  = (seq_t)891;
        cache[10] = (seq_t)2137;
        cache[11] = (seq_t)5140;
        cache[12] = (seq_t)12376;
        cache[13] = (seq_t)29756;
        cache[14] = (seq_t)71468;
        cache[15] = (seq_t)171596;
    }

    /* if requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed; return 0 to avoid undefined behavior */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* terms are indexed relative to k-1, k-2, ..., k-16 */
        seq_t val = cache[k - 1];

        val += ((seq_t)2) * cache[k - 2];
        val += cache[k - 3];
        val += cache[k - 4];
        val += ((seq_t)6) * cache[k - 5];
        val += ((seq_t)9) * cache[k - 6];
        val += ((seq_t)5) * cache[k - 7];
        val += ((seq_t)4) * cache[k - 8];
        val -= ((seq_t)3) * cache[k - 9];
        val -= cache[k - 10];
        val -= ((seq_t)2) * cache[k - 11];
        val -= cache[k - 12];
        val -= cache[k - 13];
        val -= ((seq_t)2) * cache[k - 14];
        val -= cache[k - 15];
        val -= cache[k - 16];

        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
