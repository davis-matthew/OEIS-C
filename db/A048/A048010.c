/*
 * A048010  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1} C(⌊(n‑1)/3⌋, t) * C(n‑⌊(n‑1)/3⌋, 4t) where the
 *            sum runs over all integer t such that 4t ≤ n‑⌊(n‑1)/3⌋. This
 *            counts the nonempty subsets of {1,…,n} that contain exactly 1/5
 *            of their elements among the first ⌊(n‑1)/3⌋ numbers
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n < 5 there is no non‑empty subset satisfying the condition. */
    if (n < 5) return (seq_t)0;

    /* Compute bounds */
    idx_t a_floor = (n - 1) / 3;          /* size of the first part L */
    idx_t A = a_floor;                   /* |L| */
    idx_t B = n - a_floor;               /* |R| = rest of numbers */

    /* Maximum t such that t ≤ A and 4t ≤ B  */
    idx_t tmax = A;
    if (B / 4 < tmax) tmax = B / 4;
    if (tmax == 0) return (seq_t)0;

    seq_t result = (seq_t)0;

    /* current binomial values */
    seq_t combA = (seq_t)1;   /* C(A,0) */
    seq_t combB = (seq_t)1;   /* C(B,0) */
    idx_t current_k = 0;      /* current k for combB = C(B, current_k) */

    for (idx_t t = 1; t <= tmax; ++t) {
        /* Update C(A, t) from C(A, t-1) */
        combA = combA * (seq_t)(A - t + 1) / (seq_t)t;

        /* Update C(B, 4t) from the previous needed value */
        while (current_k < 4 * t) {
            /* C(B, k+1) = C(B, k) * (B - k) / (k+1) */
            combB = combB * (seq_t)(B - current_k) / (seq_t)(current_k + 1);
            ++current_k;
        }

        /* Add term to result */
        result += combA * combB;
    }

    return result;
}
