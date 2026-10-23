/*
 * A026197  Duplicate of A026178.
 * https://oeis.org/A026197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is defined by the following procedure: * For each integer k
 *            = 1, 2, 3, … * let v1 = 3*k - 2; * output v1. * if k has not
 *            been output before, output k; otherwise output 3*k. * This
 *            generates the permutation of the positive integers described as
 *            OEIS * A026178, which is a duplicate of A026197....
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n==0, return 0 */
    idx_t pos = 1;                   /* current output position */
    idx_t k = 1;                     /* current k in the procedure */
    /* Determine an upper bound for values that will appear up to position n.
       The largest value produced comes from v = 3*k where k ≈ n/2. */
    idx_t max_k = (n + 1) / 2;        /* ceil(n/2) */
    size_t max_val = 3 * (size_t)max_k + 5;  /* safety margin */

    bool *used = (bool *)calloc(max_val + 1, sizeof(bool));
    if (used == NULL) return 0;      /* allocation failure, return 0 */

    while (1) {
        /* ---- first output of this pair: v1 = 3*k - 2 ---- */
        seq_t v1 = (seq_t)3 * (seq_t)k - (seq_t)2;
        if (pos == (idx_t)n) {
            free(used);
            return v1;
        }
        size_t idx1 = (size_t)v1;
        if (idx1 <= max_val) used[idx1] = true;
        pos++;

        /* ---- second output of this pair ---- */
        seq_t v2;
        if (!used[(size_t)k]) {
            v2 = (seq_t)k;
        } else {
            v2 = (seq_t)3 * (seq_t)k;
        }
        if (pos == (idx_t)n) {
            /* mark as used for completeness, then return */
            size_t idx2 = (size_t)v2;
            if (idx2 <= max_val) used[idx2] = true;
            free(used);
            return v2;
        }
        size_t idx2 = (size_t)v2;
        if (idx2 <= max_val) used[idx2] = true;
        pos++;

        k++;   /* proceed to next k */
    }
}
