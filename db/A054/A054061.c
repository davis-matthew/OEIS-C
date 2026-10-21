/*
 * A054061  Least k for which the integers Floor(k/(m*(m+1))) for m=1,2,...,n
 *          are distinct.
 * https://oeis.org/A054061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k such that floor(k/(m*(m+1))) for m=1..n are distinct. */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort over seq_t values */
static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    /* Pre‑compute m*(m+1) for m = 1..n */
    seq_t *a_m = (seq_t *)malloc(sizeof(seq_t) * n);
    if (!a_m) return (seq_t)0;  /* allocation failure; unlikely in tests */
    for (idx_t m = 1; m <= n; ++m) {
        a_m[m - 1] = (seq_t)m * ((seq_t)m + 1);
    }

    /* Array to hold floor(k / a_m[m]) values */
    seq_t *vals = (seq_t *)malloc(sizeof(seq_t) * n);
    if (!vals) { free(a_m); return (seq_t)0; }

    seq_t k = 1; /* start from 1; sequence is increasing anyway */
    while (1) {
        /* Compute all values for current k */
        for (idx_t i = 0; i < n; ++i) {
            vals[i] = k / a_m[i];
        }

        /* Check if all values are distinct: sort and look for duplicates */
        qsort(vals, n, sizeof(seq_t), cmp_seq);
        bool distinct = true;
        for (idx_t i = 1; i < n; ++i) {
            if (vals[i] == vals[i - 1]) {
                distinct = false;
                break;
            }
        }

        if (distinct) {
            free(a_m);
            free(vals);
            return k;
        }
        ++k;
    }
}
