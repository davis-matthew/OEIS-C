/*
 * A079967  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={4}.
 * https://oeis.org/A079967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-6), a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=8, a(5)=15
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n < 6) {
        static const seq_t base[] = { (seq_t)1, 1, 2, 4, 8, 15 };
        return base[n];
    }

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t computed_upto = 6;   /* next index that is not yet computed */

    /* Initialize the cache on first use */
    if (computed_upto == 6 && cache == NULL) {
        cache = (seq_t*)calloc(6, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – negligible in test harness */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 4;
        cache[4] = 8;
        cache[5] = 15;
    }

    /* If the requested value is already cached, return it */
    if (n < computed_upto) {
        return cache[n];
    }

    /* Grow the cache to accommodate the requested index */
    size_t new_len = computed_upto;
    while (new_len <= (size_t)n) {
        new_len <<= 1;         /* double until large enough */
    }
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;        /* allocation failure – negligible in test harness */
    cache = tmp;
    /* Zero-initialize newly allocated elements to avoid garbage */
    for (size_t i = computed_upto; i < new_len; ++i) {
        cache[i] = 0;
    }

    /* Compute the sequence from the last known index up to n */
    for (idx_t i = computed_upto; i <= n; ++i) {
        /* All indices i >= 6 are guaranteed to be >= 6 here */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-6];
    }
    computed_upto = n + 1;  /* all indices up to n are now computed */

    return cache[n];
}
