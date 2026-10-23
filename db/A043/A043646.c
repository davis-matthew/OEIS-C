/*
 * A043646  Numbers whose base-11 representation has exactly 3 runs.
 * https://oeis.org/A043646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer whose base‑11 representation has exactly   */
/*        three consecutive runs of equal digits.  The function   */
/*        generates all such numbers up to the required length,   */
/*        keeps a sorted cache, and returns the (n‑1)th element   */
/*        (since the sequence is 1‑based).                         */

#include <stdlib.h>
#include <stdbool.h>

/* Comparison routine used by qsort for seq_t values. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache holding all generated numbers, kept sorted. */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;        /* number of elements in cache   */
    static idx_t max_len   = 2;       /* length up to which we have generated (minimal is 3) */

    /* Grow the cache until it contains at least n elements. */
    while (cache_sz < n) {
        /* Generate numbers of length = max_len + 1 */
        size_t length = ++max_len;        /* next base‑11 length to process */

        /* Temporary array for the new numbers of this length */
        size_t tmp_cap = 0, tmp_sz = 0;
        seq_t *tmp = NULL;

        /* Enumerate all compositions l1 + l2 + l3 = length (each ≥ 1) */
        for (size_t l1 = 1; l1 <= length - 2; ++l1) {
            for (size_t l2 = 1; l2 <= length - l1 - 1; ++l2) {
                size_t l3 = length - l1 - l2;

                /* Leading digit cannot be 0 */
                for (int d1 = 1; d1 <= 10; ++d1) {
                    /* Second run digit (may be zero but must differ from d1) */
                    for (int d2 = 0; d2 <= 10; ++d2) {
                        if (d2 == d1) continue;
                        /* Third run digit may equal d1 or d2 only if different from d2 */
                        for (int d3 = 0; d3 <= 10; ++d3) {
                            if (d3 == d2) continue;

                            /* Build the value in base 11 */
                            seq_t val = 0;
                            for (size_t i = 0; i < l1; ++i)
                                val = val * ((seq_t)11) + ((seq_t)d1);
                            for (size_t i = 0; i < l2; ++i)
                                val = val * ((seq_t)11) + ((seq_t)d2);
                            for (size_t i = 0; i < l3; ++i)
                                val = val * ((seq_t)11) + ((seq_t)d3);

                            /* Append val to temporary array */
                            if (tmp_sz == tmp_cap) {
                                tmp_cap = tmp_cap ? tmp_cap * 2 : 8;
                                seq_t *p = (seq_t *)realloc(tmp, tmp_cap * sizeof(seq_t));
                                if (!p) exit(EXIT_FAILURE);
                                tmp = p;
                            }
                            tmp[tmp_sz++] = val;
                        }
                    }
                }
            }
        }

        /* Sort the new block */
        qsort(tmp, tmp_sz, sizeof(seq_t), cmp_seq_t);

        /* Merge sorted new numbers into the main cache */
        size_t new_cache_sz = cache_sz + tmp_sz;
        seq_t *new_cache = (seq_t *)malloc(new_cache_sz * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        size_t i = 0, j = 0, k = 0;
        while (i < cache_sz && j < tmp_sz) {
            if (cmp_seq_t(&cache[i], &tmp[j]) <= 0)
                new_cache[k++] = cache[i++];
            else
                new_cache[k++] = tmp[j++];
        }
        while (i < cache_sz) new_cache[k++] = cache[i++];
        while (j < tmp_sz)  new_cache[k++] = tmp[j++];

        /* Release old structures */
        free(cache);
        free(tmp);
        cache = new_cache;
        cache_sz = new_cache_sz;
    }

    /* Return the requested element (1‑based index). */
    return cache[(size_t)(n - 1)];
}
