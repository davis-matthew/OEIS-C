/*
 * A189886  a(n) is the number of compositions of the set {1, 2, ..., n} into
 *          blocks, each of size 1, 2 or 3 (n >= 0).
 * https://oeis.org/A189886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)/2*a(n-2) + n*(n-1)*(n-2)/6*a(n-3) with
 *            a(0)=1, a(1)=1, a(2)=3; n >= 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array: cache[i] = a(i) */
static size_t cache_len = 0;     /* number of entries stored (last valid index + 1) */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* initialise on first use */
    if (!cache) {
        cache_len = 3;                       /* indices 0..2 are valid */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);                 /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    /* expand cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure */
        cache = tmp;

        for (size_t i = cache_len; i <= n; ++i) {
            seq_t term1 = (seq_t)i * cache[i - 1];
            seq_t term2 = ((seq_t)i * (seq_t)(i - 1) / 2) * cache[i - 2];
            seq_t term3 = ((seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2) / 6) * cache[i - 3];
            cache[i] = term1 + term2 + term3;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
