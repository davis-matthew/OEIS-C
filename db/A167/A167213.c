/*
 * A167213  The number of ordered ways to achieve a score of n in darts.
 * https://oeis.org/A167213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(0) + sum_{s∈S} a(n-s) a(0) = 1 S = {1,…,20} ∪
 *            {2,4,…,40} ∪ {3,6,…,60} ∪ {25,50} (each element of S is treated
 *            as a distinct part type; duplicates contribute separate ways)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ----- Parts of the game (62 types) ----- */
    static const idx_t parts[] = {
        /* singles 1..20 */
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        /* doubles 2..40 */
        2, 4, 6, 8, 10, 12, 14, 16, 18, 20,
        22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
        /* triples 3..60 */
        3, 6, 9, 12, 15, 18, 21, 24, 27, 30,
        33, 36, 39, 42, 45, 48, 51, 54, 57, 60,
        /* bull's eye */
        25, 50
    };
    const size_t num_parts = sizeof(parts) / sizeof(parts[0]);

    /* ----- Static cache for memoisation ----- */
    static seq_t *cache = NULL;     /* array a[0..computed_len-1] */
    static size_t cache_len = 0;    /* size of allocated array    */
    static idx_t  computed_to = 0;  /* highest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 1;                     /* need entry for a(0) */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;                      /* a(0) = 1 */
        computed_to = 0;
    }

    /* Ensure array large enough for n */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure, should not happen */
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values up to n if necessary */
    if (n > computed_to) {
        for (idx_t i = computed_to + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (size_t p = 0; p < num_parts; ++p) {
                if (parts[p] <= i)
                    sum += cache[i - parts[p]];
            }
            cache[i] = sum;
        }
        computed_to = n;
    }

    return cache[n];
}
