/*
 * A049914  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2 and a(3) = 4.
 * https://oeis.org/A049914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n-1} a(i) - a(n-1-2^p) for n >= 4, * where 2^p
 *            is the largest power of two strictly less than n-1. * Initial
 *            values: a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms and prefix sum */
    static seq_t *cache = NULL;       /* a[1] .. a[computed] */
    static idx_t cache_sz = 0;        /* allocated size in elements  (>= computed+1) */
    static idx_t computed = 0;        /* largest n that has been computed */
    static seq_t sum = 0;             /* S(computed) = sum_{i=1}^{computed} a(i) */

    /* initialisation on first call */
    if (computed == 0) {
        cache_sz = 4;                         /* need indices 1..3 at least */
        cache = (seq_t *)malloc((size_t)cache_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* unlikely in the judge environment */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;

        sum = cache[1] + cache[2] + cache[3];  /* S(3) */
        computed = 3;
    }

    if (n <= computed)          /* already known */
        return cache[n];

    /* enlarge cache if necessary */
    if (cache_sz <= n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure: shouldn't happen */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute terms from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t j = i - 2;                           /* j >= 2 for i >= 4 */
        unsigned int p = 63 - __builtin_clzll(j);  /* floor(log2(j)) */
        idx_t pow2 = 1ULL << p;
        idx_t m = (i - 1) - pow2;                  /* guaranteed >=1 */

        seq_t a_m = cache[m];
        seq_t a_i = sum - a_m;                     /* a(i) = S(i-1) - a(m) */
        cache[i] = a_i;

        sum += a_i;                                /* update prefix sum */
    }

    computed = n;
    return cache[n];
}
