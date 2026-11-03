/*
 * A052050  Squares whose digits occur with an equal minimal frequency of 2.
 * https://oeis.org/A052050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052050: Squares whose digits occur with an equal minimal frequency of 2.
   The code searches increasing squares and tests the property. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: test whether a square number satisfies the sequence condition. */
static bool square_has_minimal_two(seq_t sq)
{
    int cnt[10];
    memset(cnt, 0, sizeof(cnt));

    /* Count decimal digits of sq. */
    seq_t v = sq;
    while (v > 0) {
        int d = (int)(v % 10);
        cnt[d]++;
        v /= 10;
    }

    /* Find min>0 and max counts among digits that appear. */
    int min_pos = -1;
    int max_pos = -1;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] > 0) {
            if (min_pos == -1 || cnt[d] < min_pos) min_pos = cnt[d];
            if (max_pos == -1 || cnt[d] > max_pos) max_pos = cnt[d];
        }
    }
    /* Accept if every non‑zero count equals 2. */
    return (min_pos == max_pos && min_pos == 2);
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Static state for incremental computation. */
    static seq_t *terms = NULL;          /* array of found terms */
    static size_t capacity = 0;          /* allocated capacity */
    static size_t length = 0;            /* number of terms stored */
    static idx_t last_k = 0;             /* last k processed (k^2) */

    /* Grow the terms array when needed. */
    if (capacity == 0) {
        capacity = 1024;
        terms = mem_new(seq_t, capacity);
    }

    /* Generate squares until we have at least n terms. */
    while ((idx_t)length < n) {
        last_k += 1;
        seq_t sq = ((seq_t)last_k) * ((seq_t)last_k);
        if (square_has_minimal_two(sq)) {
            /* Append to the array. */
            if (length == capacity) {
                capacity *= 2;
                terms = mem_resize(seq_t, terms, capacity);
            }
            terms[length++] = sq;
        }
    }

    return terms[n - 1];
}
