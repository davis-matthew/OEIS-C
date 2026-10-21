/*
 * A062554  Products of numbers of faces of Platonic solids, i.e., of
 *          {4,6,8,12,20}.
 * https://oeis.org/A062554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the numbers of faces of the Platonic solids
 *            {4, 6, 8, 12, 20}; the sequence consists of all distinct finite
 *            products of these numbers, sorted in increasing order (the empty
 *            product 1 is omitted)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t generators[5] = {4, 6, 8, 12, 20};

seq_t a(idx_t n)
{
    /* static cache of generated sequence values (index 0 is the sentinel 1) */
    static seq_t *seq = NULL;          /* array of sequence values */
    static idx_t *idx = NULL;          /* indices for each generator */
    static idx_t seq_len = 0;          /* number of stored values (including sentinel) */
    static idx_t cache_cap = 0;        /* current capacity of seq array */

    /* initialise static data on first entry */
    if (!seq) {
        cache_cap = 1024;              /* initial capacity */
        seq = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        idx = (idx_t *)malloc(5 * sizeof(idx_t));
        seq[0] = (seq_t)1;            /* sentinel: product of zero generators */
        seq_len = 1;
        for (int i = 0; i < 5; ++i) idx[i] = 0;
    }

    /* generate sequence values until we have the n‑th term (offset 1) */
    while (seq_len <= n) {
        /* ensure enough capacity */
        if (seq_len == cache_cap) {
            cache_cap <<= 1;
            seq = (seq_t *)realloc(seq, cache_cap * sizeof(seq_t));
        }

        /* find the smallest next candidate */
        seq_t cand = seq[idx[0]] * generators[0];
        for (int i = 1; i < 5; ++i) {
            seq_t v = seq[idx[i]] * generators[i];
            if (v < cand) cand = v;
        }

        /* append the chosen candidate */
        seq[seq_len++] = cand;

        /* advance all indices that produced this candidate */
        for (int i = 0; i < 5; ++i) {
            if (seq[idx[i]] * generators[i] == cand)
                ++idx[i];
        }
    }

    return seq[n];
}
