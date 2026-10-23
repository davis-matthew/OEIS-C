/*
 * A133270  Groups of 4 numbers describing a sequence of Minor 7th chords by
 *          assigning numbers 1 to 12 to an octave.
 * https://oeis.org/A133270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A133270: Groups of 4 numbers describing Minor 7th chords.
 *
 * Construction:
 *   Let c(i) for 1 ≤ i ≤ 12 be the 12 chords
 *     1 → {1, 4, 8, 11}, 2 → {2, 5, 9, 12}, 3 → {3, 6, 10, 1},
 *     4 → {4, 7, 11, 2}, 5 → {5, 8, 12, 3}, 6 → {6, 9, 1, 4},
 *     7 → {7, 10, 2, 5}, 8 → {8, 11, 3, 6}, 9 → {9, 12, 4, 7},
 *     10 → {10, 3, 5, 8}, 11 → {11, 4, 6, 9}, 12 → {12, 5, 7, 10}.
 *   The sequence starts with the 4 numbers of c(1).
 *   Whenever a(n) for n ≥ 2 is required, read the current value
 *   a(n) (the chord index) and append the 4 numbers of c(a(n))
 *   to the end of the sequence.
 *   This produces a growing 4‑column table of chord indices.
 *
 * The algorithm below memoises the sequence so that any a(n)
 * (for n ≥ 1) can be obtained in O(n) time and O(n) memory
 * regardless of call order.
 */

#include <stdlib.h>   /* abort, realloc */

/* 12 chords – stored as uint64_t indices for easy array access.
 *  Index 0 is unused to allow 1‑based indexing.             */
static const idx_t chords[13][4] = {
    {0,0,0,0},
    {1,4,8,11},
    {2,5,9,12},
    {3,6,10,1},
    {4,7,11,2},
    {5,8,12,3},
    {6,9,1,4},
    {7,10,2,5},
    {8,11,3,6},
    {9,12,4,7},
    {10,3,5,8},
    {11,4,6,9},
    {12,5,7,10}
};

/* Memoised cache of computed sequence values.               */
static seq_t *seq_cache = NULL;   /* ptr to allocated array   */
static idx_t  cache_len   = 0;     /* number of elements seen */
static size_t cache_cap   = 0;     /* allocated capacity      */
static idx_t  next_source = 2;     /* index whose value is the
                                     chord number to append next */

/* Ensure the cache can hold at least 'cap' elements.          */
static void ensure_cache_capacity(size_t cap)
{
    if (cache_cap >= cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory allocation failure */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A133270 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) abort();        /* offset is 1 – invalid index */

    /* Initialise cache on first use.                          */
    if (cache_len == 0) {
        ensure_cache_capacity(4);
        seq_cache[0] = (seq_t)chords[1][0];
        seq_cache[1] = (seq_t)chords[1][1];
        seq_cache[2] = (seq_t)chords[1][2];
        seq_cache[3] = (seq_t)chords[1][3];
        cache_len   = 4;
        next_source = 2;       /* a(2) is the first chord index to use */
    }

    /* Extend the sequence until it covers the requested index. */
    while (cache_len < n) {
        /* Retrieve the chord number at position next_source - 1. */
        seq_t chord_seq = seq_cache[next_source - 1];
        idx_t chord_idx = (idx_t)chord_seq;   /* 1 … 12 */

        if (chord_idx < 1 || chord_idx > 12) abort();

        /* Append the 4 numbers of the chosen chord. */
        ensure_cache_capacity(cache_len + 4);
        seq_cache[cache_len++] = (seq_t)chords[chord_idx][0];
        seq_cache[cache_len++] = (seq_t)chords[chord_idx][1];
        seq_cache[cache_len++] = (seq_t)chords[chord_idx][2];
        seq_cache[cache_len++] = (seq_t)chords[chord_idx][3];

        next_source++;          /* next term will be used next time   */
    }

    return seq_cache[n - 1];
}
