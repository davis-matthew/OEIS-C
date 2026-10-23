/*
 * A107957  Partial sums of A107947.
 * https://oeis.org/A107957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} b(k) , * where b(0) = 1 and for k ≥ 1: * b(k)
 *            = b(k-1) + b(k - 2^{⌊log₂ k⌋}) (with b(0) = 1)
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the partial sums of A107947. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *diff = NULL;     /* b(k) values */
    static seq_t *psum = NULL;     /* a(k) values */
    static idx_t cap = 0;          /* allocated size */

    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *diff_new = (seq_t *)realloc(diff, new_cap * sizeof(seq_t));
        seq_t *psum_new = (seq_t *)realloc(psum, new_cap * sizeof(seq_t));
        if (!diff_new || !psum_new) {
            /* Allocation failure – terminate. */
            /* In production code you might return an error code instead. */
            exit(EXIT_FAILURE);
        }
        diff = diff_new;
        psum = psum_new;

        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                diff[0] = (seq_t)1;
                psum[0] = diff[0];
            } else {
                /* Find largest power of 2 ≤ i. */
                idx_t p = 1;
                while (p <= i / 2) p <<= 1;
                diff[i] = diff[i - 1] + diff[i - p];
                psum[i] = psum[i - 1] + diff[i];
            }
        }
        cap = new_cap;
    }
    return psum[n];
}
