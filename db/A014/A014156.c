/*
 * A014156  Numbers that are not the sum of a nonnegative cube and a
 *          triangular number.
 * https://oeis.org/A014156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer that is NOT representable as a non‑negative
 * cube plus a triangular number (OEIS A014156) by a brute‑force sieve.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_cube_root.h"
#include "../utils/floor_sqrt.h"

static seq_t *cache      = NULL;  /* list of non‑representable numbers   */
static size_t cache_len  = 0;      /* current list length                 */
static idx_t  cache_lim  = 0;      /* current sieve limit                 */

/* Build a list of all numbers ≤ limit that cannot be written
   as cube + triangular.  Return the list and its length.   */
static seq_t *build_nonrep(idx_t limit, size_t *out_len)
{
    unsigned char *rep = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!rep) return NULL;

    idx_t cube_lim = int_cube_root(limit);
    for (idx_t i = 0; i <= cube_lim; ++i) {
        seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;
        seq_t remain = (seq_t)limit - cube;          /* remain ≥ 0            */
        seq_t eight_rem_plus_one = remain * 8 + 1;    /* used for sqrt test    */
        seq_t sq = floor_sqrt(eight_rem_plus_one);
        idx_t tri_lim = (idx_t)((sq - 1) / 2);

        for (idx_t j = 0; j <= tri_lim; ++j) {
            seq_t tri = (seq_t)j * (seq_t)(j + 1) / 2;
            idx_t idx = (idx_t)(cube + tri);
            rep[(size_t)idx] = 1;
        }
    }

    /* Count numbers that are NOT marked.      */
    size_t count = 0;
    for (idx_t k = 1; k <= limit; ++k)
        if (!rep[(size_t)k]) ++count;

    /* Create array of the missing numbers.     */
    seq_t *vec = (seq_t *)malloc(count * sizeof(seq_t));
    if (!vec) {
        free(rep);
        return NULL;
    }

    size_t pos = 0;
    for (idx_t k = 1; k <= limit; ++k)
        if (!rep[(size_t)k])
            vec[pos++] = (seq_t)k;

    free(rep);
    *out_len = count;
    return vec;
}

/* Ensure that cache contains at least n elements. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Start with a modest limit and double until we have enough terms. */
    idx_t limit = (idx_t)(n * 10);
    if (limit < (idx_t)1000) limit = 1000;

    while (cache_len < (size_t)n) {
        size_t new_len = 0;
        seq_t *new_buf  = build_nonrep(limit, &new_len);
        if (!new_buf)
            return;                    /* unlikely, but avoid crash */

        free(cache);
        cache     = new_buf;
        cache_len = new_len;
        cache_lim = limit;

        if (cache_len >= (size_t)n)
            return;
        limit <<= 1;                    /* double the sieve limit   */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
