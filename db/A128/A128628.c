/*
 * A128628  An irregular triangular array read by rows, with shape sequence
 *          A000041(n) related to sequence A060850.
 * https://oeis.org/A128628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the number of parts of the n-th partition when all
 *            integer partitions are listed in reverse‑lexicographic order,
 *            concatenated by row (row m contains all partitions of m)
 * Range:     u8 n <= 9295, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reallocate an array of seq_t values. */
static seq_t *seq_realloc(seq_t *old, size_t /*old_cap*/, size_t new_cap)
{
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
}

/* Generate all partitions of 'rem' into parts not exceeding 'max_part'.
   For each finished partition, record its number of parts into the cache. */
static void gen_partitions(idx_t rem, idx_t max_part, idx_t parts,
                           seq_t **cache, size_t *len, size_t *cap)
{
    if (rem == 0) {
        /* Ensure capacity for the new value. */
        if (*len >= *cap) {
            size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
            seq_t *tmp = seq_realloc(*cache, *cap, new_cap);
            *cache = tmp;
            *cap = new_cap;
        }
        (*cache)[*len] = (seq_t)parts;
        ++(*len);
        return;
    }
    for (idx_t part = max_part; part >= 1; --part) {
        if (part > rem) continue;
        gen_partitions(rem - part, part, parts + 1, cache, len, cap);
    }
}

/* Generate all partitions of integer 'm' and append the part counts to the cache. */
static void generate_row(idx_t m, seq_t **cache, size_t *len, size_t *cap)
{
    gen_partitions(m, m, 0, cache, len, cap);
}

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_row = 0;

    if (n == 0) return 0;               /* safety for accidental zero index */

    /* Generate additional rows as needed to reach the requested index. */
    while (cache_len < n) {
        ++last_row;
        generate_row(last_row, &cache, &cache_len, &cache_cap);
    }

    return cache[n - 1];
}
