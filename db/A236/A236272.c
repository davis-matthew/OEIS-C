/*
 * A236272  Number of (n+1)X(2+1) 0..2 arrays with the upper median unequal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A236272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) -189*a(n-3) +169*a(n-4) +720*a(n-5) *
 *            -498*a(n-6) -680*a(n-7) +48*a(n-8) +48*a(n-9) * with initial
 *            values * a(1)=208, a(2)=1770, a(3)=15434, a(4)=137142,
 *            a(5)=1226262, * a(6)=11005062, a(7)=98896922, a(8)=889370642,
 *            a(9)=8000222410
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated size */
    static size_t computed = 0;    /* highest computed index */

    /* cannot compute for n = 0; simply return 0 */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache with the first 9 terms */
    if (!cache) {
        cache_len = 10;                        /* indices 0..9 inclusive */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)208ULL;
        cache[2] = (seq_t)1770ULL;
        cache[3] = (seq_t)15434ULL;
        cache[4] = (seq_t)137142ULL;
        cache[5] = (seq_t)1226262ULL;
        cache[6] = (seq_t)11005062ULL;
        cache[7] = (seq_t)98896922ULL;
        cache[8] = (seq_t)889370642ULL;
        cache[9] = (seq_t)8000222410ULL;
        computed = 9;
    }

    /* if requested value already computed, return it */
    if (n <= computed)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    size_t target = (size_t)n + 1;
    if (target > cache_len) {
        size_t new_len = cache_len;
        while (new_len < target) new_len *= 2;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* compute terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        /* sum of positive contributions */
        seq_t pos = cache[i-1] * (seq_t)11ULL;
        pos += cache[i-4] * (seq_t)169ULL;
        pos += cache[i-5] * (seq_t)720ULL;
        pos += cache[i-8] * (seq_t)48ULL;
        pos += cache[i-9] * (seq_t)48ULL;

        /* sum of negative contributions */
        seq_t neg = cache[i-3] * (seq_t)189ULL;
        neg += cache[i-6] * (seq_t)498ULL;
        neg += cache[i-7] * (seq_t)680ULL;

        /* result = positive - negative (positive >= negative) */
        cache[i] = pos - neg;
    }

    computed = (size_t)n;
    return cache[(size_t)n];
}
