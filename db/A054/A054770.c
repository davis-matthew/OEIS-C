/*
 * A054770  Numbers that are not the sum of distinct Lucas numbers 1,3,4,7,11,
 *          ... (A000204).
 * https://oeis.org/A054770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000201(n) + 2*n - 1 * * A000201(n) is the lower Wythoff
 *            sequence (floor(n*phi)). * The routine builds the first n values
 *            of this sequence * using the classical construction a_k =
 *            smallest unused integer, * b_k = a_k + k. It then returns
 *            A000201(n) + 2*n - 1. * Complexity: O(n) time and O(...
 * Range:     u8 n <= 71, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; n = 0 is outside the range */
        return 0;

    /* Upper bound for all numbers that may appear in the construction:
     * a_k ≈ φ k  (≈1.618 k) and b_k = a_k + k ≈ (φ+1)k (≈2.618 k).
     * 4*n + 10 is a generous safe limit for all n ≥ 1. */
    size_t N     = (size_t)n;
    size_t limit = 4 * N + 10;

    /* Array marking which integers have already appeared as a_k or b_k. */
    bool *used = (bool *)calloc(limit + 1, sizeof(bool));
    if (!used)
        return 0;                  /* allocation failed – unlikely */

    /* Storage for the lower Wythoff numbers a_k. */
    seq_t *lower = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!lower) {
        free(used);
        return 0;
    }

    size_t cur = 1;                /* smallest candidate not yet used */
    for (size_t k = 1; k <= N; ++k) {
        while (used[cur])
            ++cur;                 /* skip numbers already taken */

        seq_t a_k = (seq_t)cur;    /* current lower Wythoff number */
        lower[k] = a_k;

        used[cur] = true;          /* mark a_k as used */

        size_t b_val = cur + k;    /* corresponding upper Wythoff number */
        used[b_val] = true;        /* guaranteed to be ≤ limit */
    }

    /* a(n) = A000201(n) + 2*n - 1. */
    seq_t result = lower[N] + (seq_t)n * 2 - (seq_t)1;

    free(used);
    free(lower);
    return result;
}
