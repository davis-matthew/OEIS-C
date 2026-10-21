/*
 * A050031  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A050031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 127, u64 n <= 506,
 *            u128 n <= 3089, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A050031
 * Recurrence: a(1)=1, a(2)=1, a(3)=2,
 *             for n>=4: a(n) = a(n-1) + a(m)
 *             where m = 2*n - 2 - 2^(p+1),
 *             p = floor(log2(n-2))  (i.e. 2^p < n-1 <= 2^(p+1))
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base cases */
    if (n <= 3) {
        if (n == 1 || n == 2) return (seq_t)1;
        /* n == 3 */
        return (seq_t)2;
    }

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* allocated length */
    static idx_t computed   = 0;    /* highest n for which cache[n] is valid */

    /* grow cache if needed */
    if (cache_len <= n) {
        idx_t new_len = n * 2 + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }

    /* initialize cache[1..3] if not done yet */
    if (computed < 3) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        computed = 3;
    }

    /* compute up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* compute p = floor(log2(i-2)) */
        idx_t v = i - 2;     /* i>=4 so v>=2 */
        int p = 0;
        /* find largest p such that 2^p < i-1 <= 2^(p+1) */
        while ((idx_t)1 << (p + 1) <= v)
            ++p;

        /* pow2 = 2^(p+1) */
        seq_t pow2 = ((seq_t)1) << (p + 1);

        /* m = 2*i - 2 - 2^(p+1) */
        /* pow2 fits in idx_t for all n used in tests */
        idx_t m = (idx_t)(2 * i - 2) - (idx_t)pow2;

        /* recurrence */
        cache[i] = cache[i - 1] + cache[m];
    }

    computed = n;
    return cache[n];
}
