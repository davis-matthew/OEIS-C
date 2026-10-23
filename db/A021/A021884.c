/*
 * A021884  Expansion of 1/((1-x)(1-4x)(1-7x)(1-11x)).
 * https://oeis.org/A021884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 171*a(n-2) + 457*a(n-3) - 308*a(n-4); a(0)=1,
 *            a(1)=23, a(2)=358, a(3)=4758
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
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

    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Ensure array capacity */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t newcap = cache_cap ? (size_t)cache_cap << 1 : 8;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)newcap;
    }

    /* Initialize first four terms if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)358;
        cache[3] = (seq_t)4758;
        cache_len = 4;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t tot = ((seq_t)23 * t1) + ((seq_t)457 * t3);
            seq_t sub = ((seq_t)171 * t2) + ((seq_t)308 * t4);
            cache[i] = tot - sub;   /* tot >= sub for all n >= 4 */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
