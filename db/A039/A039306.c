/*
 * A039306  Number of distinct quadratic residues mod 9^n.
 * https://oeis.org/A039306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 9*a(n-2) Initial values: a(0) = 1, a(1) = 4
 *            This implements OEIS sequence A039306 (number of distinct
 *            quadratic residues mod 9^n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* array holding precomputed terms */
static size_t cache_len = 0;       /* current length of cache (elements 0 .. cache_len-1 */ 

seq_t a(idx_t n) {
    /* Ensure cache has room for n */
    size_t target = (size_t)n;  /* n fits into size_t for indices up to ~10 000 */

    if (cache_len <= target) {
        size_t new_len = target + 1;                     /* need indices 0 .. target */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        if (!new_cache) { abort(); }                     /* allocation failed */
        cache = new_cache;

        /* Compute missing terms (from old cache_len up to target) */
        for (size_t i = cache_len; i <= target; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;                     /* a(0) = 1 */
            } else if (i == 1) {
                cache[i] = (seq_t)4;                     /* a(1) = 4 */
            } else {
                cache[i] = cache[i - 1] * (seq_t)10
                           - cache[i - 2] * (seq_t)9;    /* a(i) = 10*a(i-1) - 9*a(i-2) */
            }
        }
        cache_len = new_len;
    }

    return cache[target];
}
