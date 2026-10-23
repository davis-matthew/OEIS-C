/*
 * A131192  Numbers n >= 0 such that d(n) = (n^1 + 1)*(n^2 + 2)*...*(n^26 +
 *          26) / 26! is nonintegral.
 * https://oeis.org/A131192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_nonintegral
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS A131192 sequence.
   The elements are the integers n ≥ 0 for which
   d(n) = Πₖ₌₁²⁶ (n^k + k) / 26! is not an integer.
   This is equivalent to the union of the arithmetic progressions
   {11m + 7} \ {121m + 117} and {13m + 11} \ {169m + 63}.
   The implementation tests each integer in turn and memoises the
   results, so a(n) can be queried in any order. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_nonintegral.h"

/* Memoised storage for the sequence values. */
static seq_t *seq = NULL;        /* array of stored sequence values   */
static size_t seq_len = 0;       /* number of values stored           */
static size_t seq_cap = 0;       /* current capacity of the array     */
static idx_t next_test = 0;      /* next integer to test for membership */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for offset 1, but safe return */

    /* Grow the array until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Test successive integers for membership. */
        if (is_nonintegral(next_test)) {
            /* Ensure capacity for the new element. */
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 4 : seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)next_test;  /* store the term */
        }
        ++next_test;
    }

    return seq[n - 1];
}
