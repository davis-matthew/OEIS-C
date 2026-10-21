/*
 * A205118  s(k)-s(j), where (s(k),s(j)) is the least pair of Lucas numbers
 *          for which n divides their difference.
 * https://oeis.org/A205118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = minimal positive difference L_i - L_j for Lucas numbers
   L_k with L_i ≡ L_j (mod n).  This is A205118. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but return 0 */

    /* Allocate one entry per residue class modulo n. */
    seq_t *first_val = seq_alloc_new_array((size_t)n);
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!first_val || !seen) {
        free(first_val);
        free(seen);
        return 0;
    }

    seq_t best_diff = 0;                 /* 0 = not yet set */
    /* Initial Lucas numbers L_0 = 1, L_1 = 3. */
    seq_t L0 = (seq_t)1;
    seq_t L1 = (seq_t)3;
    idx_t r0 = (idx_t)1 % n;
    idx_t r1 = (idx_t)3 % n;

    /* Process L_0. */
    if (!seen[r0]) {
        seen[r0] = true;
        first_val[r0] = L0;
    }

    /* Process L_1. */
    if (!seen[r1]) {
        seen[r1] = true;
        first_val[r1] = L1;
    } else {
        seq_t diff = L1 - first_val[r1];
        if (best_diff == 0 || diff < best_diff) best_diff = diff;
    }

    /* Generate further Lucas numbers up to n+1 terms (pigeonhole gives a
       repetition). */
    seq_t prev = L0, curr = L1;
    idx_t mr = r0, mc = r1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;          /* L_i = L_{i-1} + L_{i-2} */
        idx_t mn = mr + mc;
        if (mn >= n) mn -= n;             /* (mr + mc) mod n */

        if (!seen[mn]) {
            seen[mn] = true;
            first_val[mn] = next;
        } else {
            seq_t diff = next - first_val[mn];
            if (best_diff == 0 || diff < best_diff) best_diff = diff;
        }

        /* Rotate */
        prev = curr;
        curr = next;
        mr = mc;
        mc = mn;
    }

    free(first_val);
    free(seen);
    return best_diff;
}
