/*
 * A159967  Numerator of Hermite(n, 7/24).
 * https://oeis.org/A159967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7 · a(n‑1) – 288 · (n‑1) · a(n‑2), a(0)=1, a(1)=7
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A159967 (numerator of Hermite(n,7/24)). */

#include <stdbool.h>   /* for bool type, if needed */
#include <stdint.h>    /* for uint64_t type, if needed */

seq_t a(idx_t n) {
    const seq_t c7   = (seq_t)7;
    const seq_t c288 = (seq_t)288;

    /* Handle the first two base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Use a simple linear recurrence with O(n) time and O(1) space. */
    seq_t prev_prev = (seq_t)1;   /* a(0) */
    seq_t prev      = (seq_t)7;   /* a(1) */
    seq_t curr;

    /* Loop from i = 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        curr = c7 * prev - c288 * (seq_t)(i - 1) * prev_prev;
        prev_prev = prev;
        prev      = curr;
    }

    return curr;
}
