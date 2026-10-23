/*
 * A167167  A001045 with a(0) replaced by -1.
 * https://oeis.org/A167167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = a(n‑1) + 2*a(n‑2) for n ≥ 3 with initial values a(0) =
 *            –1, a(1) = 1, a(2) = 1. All operations are performed in seq_t,
 *            chosen by the build
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* Recurrence: a(k) = a(k‑1) + 2*a(k‑2) for k ≥ 3 */
    seq_t a0 = (seq_t)-1;   /* a(0) */
    seq_t a1 = (seq_t)1;    /* a(1) */
    seq_t a2 = (seq_t)1;    /* a(2) */

    for (idx_t k = 3; k <= n; ++k)
    {
        /* a_next = a2 + 2*a1  (avoid multiplication, use addition) */
        seq_t a_next = a2 + a1 + a1;
        a0 = a1;          /* shift the window */
        a1 = a2;
        a2 = a_next;
    }

    return a2;
}
