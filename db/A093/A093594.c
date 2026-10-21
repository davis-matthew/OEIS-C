/*
 * A093594  Farey-factorial numerators, including duplicates.
 * https://oeis.org/A093594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   factorial, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by incremental generation of rows of Farey-fraction numerators multiplied by factorial. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_int.h"
#include "../utils/factorial.h"

/* Structure to hold a reduced fraction for sorting */
typedef struct {
    unsigned int p;
    unsigned int q;
} frac_t;

/* Comparator for fractions a/b < c/d */
static int cmp_frac(const void *x, const void *y) {
    const frac_t *A = (const frac_t *)x;
    const frac_t *B = (const frac_t *)y;
    long long lhs = (long long)A->p * B->q;
    long long rhs = (long long)B->p * A->q;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

/* Returns the n-th term of the concatenated sequence of
   n! * Farey fractions of order n (including duplicates). */
seq_t a(idx_t n) {
    /* Lazy initialization of static storage */
    static seq_t *seq_vals = NULL;     /* flattened sequence values   */
    static size_t seq_len = 0;         /* number of values stored     */
    static size_t seq_cap = 0;         /* allocated capacity          */
    static idx_t curr_row = 0;         /* last processed row index    */
    static seq_t curr_fact = 1;        /* factorial of curr_row       */
    static bool initialized = false;   /* one‑time init flag          */

    if (!initialized) {
        seq_vals = NULL;
        seq_len = 0;
        seq_cap = 0;
        curr_row = 0;
        curr_fact = 1;
        initialized = true;
    }

    /* Generate rows until we have at least n terms */
    while (seq_len < n) {
        /* Move to next row and update factorial */
        curr_row++;
        curr_fact *= (seq_t)curr_row;

        /* Allocate temporary array for fractions of this row */
        size_t maxPairs = (size_t)curr_row * (curr_row + 1) / 2;
        frac_t *row = (frac_t *)malloc(maxPairs * sizeof(frac_t));
        if (!row) return (seq_t)0; /* on allocation failure return 0 (unlikely) */

        /* Generate all reduced fractions p/q, 1 <= p <= q <= curr_row */
        size_t cnt = 0;
        for (unsigned int q = 1; q <= curr_row; ++q) {
            for (unsigned int p = 1; p <= q; ++p) {
                if (gcd_int(p, q) == 1) {
                    row[cnt].p = p;
                    row[cnt].q = q;
                    ++cnt;
                }
            }
        }

        /* Sort the fractions by value p/q */
        qsort(row, cnt, sizeof(frac_t), cmp_frac);

        /* Append the products n! * p / q to the flattened sequence */
        for (size_t i = 0; i < cnt; ++i) {
            seq_t val = (curr_fact * (seq_t)row[i].p) / (seq_t)row[i].q;
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = val;
        }

        free(row);
    }

    /* Return the requested element (1‑based index) */
    return seq_vals[n - 1];
}
