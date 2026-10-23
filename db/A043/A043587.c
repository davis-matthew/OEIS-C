/*
 * A043587  Numbers whose base-3 representation has exactly 7 runs.
 * https://oeis.org/A043587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   this function generates the (n+0)-th integer whose base‑3
 *            representation has exactly 7 runs (maximal blocks of equal
 *            digits). The OEIS sequence A043587 lists these numbers with
 *            offset 1
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gen_compositions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gen_compositions.h"

static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

static size_t n_choose_k(size_t n, size_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    size_t r = 1;
    for (size_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* Append all numbers of length D to the global cache.
   The cache is kept sorted because every number with more digits
   is larger than any number with fewer digits. */
static void add_length(idx_t D,
                       seq_t **cache, size_t *len, size_t *cap)
{
    size_t comp = n_choose_k((size_t)(D - 1), 6);
    size_t pats = comp * 128;
    seq_t *tmp = (seq_t *)malloc(pats * sizeof(seq_t));
    size_t ppos = 0;
    int lens[7];
    gen_compositions((int)D, 0, lens, tmp, &ppos);
    qsort(tmp, pats, sizeof(seq_t), cmp_seq_t);

    if (*len + pats > *cap) {
        size_t newcap = (*cap == 0) ? 128 : *cap * 2;
        while (newcap < *len + pats) newcap <<= 1;
        seq_t *newc = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cache = newc;
        *cap = newcap;
    }
    memcpy(*cache + *len, tmp, pats * sizeof(seq_t));
    *len += pats;
    free(tmp);
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds, but function may be called with n=0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current_maxD = 6;   /* we have generated up to 6 digits (none) */

    while (cache_len < (size_t)n) {
        ++current_maxD;          /* next digit length to generate */
        add_length(current_maxD, &cache, &cache_len, &cache_cap);
    }
    return cache[n - 1];   /* 1‑based indexing */
}
