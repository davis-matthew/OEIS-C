/*
 * A043581  Numbers whose base-2 representation has exactly 14 runs.
 * https://oeis.org/A043581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   comb64, min_length_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose binary representation has exactly 14 runs (search). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/comb64.h"
#include "../utils/min_length_for.h"

/* Compare two seq_t values for qsort. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Generate all numbers having exactly 14 runs, with binary lengths 14..Lmax.
   The resulting array is sorted in ascending order. */
static void generate_all(idx_t n, seq_t **out, size_t *len)
{
    int Lmax = min_length_for(n);
    *len = (size_t)comb64(Lmax, 14);
    seq_t *arr = (seq_t *)malloc(*len * sizeof(seq_t));
    if (!arr) abort();

    size_t pos = 0;
    /* For each possible length L, iterate all combinations of cuts. */
    for (int L = 14; L <= Lmax; ++L) {
        int k = 13;                 /* number of cuts required to get 14 runs */
        int m = L - 1;              /* available cut positions: 0..m-1 */

        int comb[13];
        /* first combination: 0,1,...,k-1 */
        for (int i = 0; i < k; ++i) comb[i] = i;

        while (1) {
            /* Build the binary number defined by this combination. */
            seq_t val = 0;
            seq_t bit = 1;          /* the first run starts with 1 */
            int cutIdx = 0;
            for (int i = 0; i < L; ++i) {
                val = (val << 1) | bit;
                if (cutIdx < k && i == comb[cutIdx]) {
                    bit ^= 1;
                    ++cutIdx;
                }
            }
            arr[pos++] = val;

            /* Advance to next combination in lexicographic order. */
            int j;
            for (j = k - 1; j >= 0; --j) {
                if (comb[j] != j + m - k) break;
            }
            if (j < 0) break;  /* all combinations processed */
            ++comb[j];
            for (int i = j + 1; i < k; ++i)
                comb[i] = comb[i - 1] + 1;
        }
    }

    /* Sort the array in ascending numeric order. */
    qsort(arr, *len, sizeof(seq_t), cmp_seq_t);
    *out = arr;
}

/* Cached sequence and its length. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the nth term (1‑based index) of A043581. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for indices below offset */

    /* Generate and cache more terms if needed. */
    if (cache_len < (size_t)n) {
        seq_t *new_cache;
        size_t new_len;
        generate_all(n, &new_cache, &new_len);
        free(cache);         /* free old cache, if any */
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n - 1];          /* one‑based indexing */
}
