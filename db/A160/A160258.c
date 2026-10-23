/*
 * A160258  The entries of A033942, repeated for each different factorization
 *          into 3 factors larger than 1.
 * https://oeis.org/A160258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest product of three integers > 1, counted
 *            once for each unordered triple. That is, for every unordered
 *            triple (c ≤ b ≤ a, a,b,c ≥ 2) with product p = a*b*c, the value
 *            p appears in the sequence exactly once for each such triple. The
 *            sequence is sorted in non‑decreasing order
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* comparison function for qsort */
static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* static cache of generated sequence */
static seq_t *cache = NULL;          /* cache array */
static size_t cache_len = 0;          /* number of elements in cache */
static uint64_t prod_limit = 8;       /* current product upper bound */

/* ensure that cache contains at least n elements (n >= 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    while (cache_len < (size_t)n) {
        /* increase the product bound, with saturation at UINT64_MAX */
        if (prod_limit > UINT64_MAX / 2)
            prod_limit = UINT64_MAX;
        else
            prod_limit *= 2;

        unsigned __int128 limit128 = (unsigned __int128)prod_limit;

        /* dynamic array for triple products */
        size_t cap = 0, len = 0;
        seq_t *arr = NULL;

        for (uint64_t c = 2;
             (unsigned __int128)c * c * c <= limit128;
             ++c) {
            for (uint64_t b = c;
                 (unsigned __int128)b * b * c <= limit128;
                 ++b) {
                uint64_t max_a = (uint64_t)(limit128 / ((unsigned __int128)b * c));
                for (uint64_t a = b; a <= max_a; ++a) {
                    unsigned __int128 prod128 =
                        (unsigned __int128)a * b * c;
                    if (prod128 > limit128) break;

                    /* grow array as needed */
                    if (len == cap) {
                        cap = (cap == 0) ? 16 : cap * 2;
                        arr = (seq_t *)realloc(arr,
                                              sizeof(seq_t) * cap);
                    }

                    /* store product (truncated to seq_t) */
                    uint64_t prod_u64 = (uint64_t)prod128;
                    arr[len++] = (seq_t)prod_u64;
                }
            }
        }

        /* sort the array */
        qsort(arr, len, sizeof(seq_t), compare_seq_t);

        /* replace old cache */
        free(cache);
        cache = arr;
        cache_len = len;
    }
}

/* Sequence function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined behaviour for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
