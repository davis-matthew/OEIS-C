/*
 * A135022  Define a sequence of binary words by w(1)=10 and
 *          w(n+1)=w(n)w(n)Reverse[w(n)]. Sequence gives the limiting word
 *          w(infinity).
 * https://oeis.org/A135022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  w₁ = 10
 *                w_{k+1} = w_k | w_k | Reverse[w_k]
 * a(n) is the n‑th bit of the limit w(∞).
 * Base: a(1) = 1, a(2) = 0.
 * The algorithm recurses on the construction of w_k:
 *   if n is in the third block it is reflected in the first block.
 *   for k > 1 we reduce the problem to w_{k‑1}.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)(n == 1 ? 1 : 0);
    }

    /* Find k such that len(k) >= n, where len(1)=2 and len(k+1)=3*len(k) */
    idx_t len = 2; /* len(k) */
    idx_t k = 1;
    while (len < n) {
        len *= 3;
        ++k;
    }

    /* Reduce (n, k, len) to the base word w₁ = 10 */
    while (k > 1) {
        idx_t half = len / 3; /* len(k-1) */
        if (n <= half) {
            /* first block – nothing changes */
        } else if (n <= 2 * half) {
            /* second block – shift index into the first block */
            n -= half;
        } else {
            /* third (reversed) block */
            n = half - (n - 2 * half) + 1;
        }
        len = half;
        --k;
    }

    /* Now k == 1; w₁ = "10" */
    return (seq_t)(n == 1 ? 1 : 0);
}
