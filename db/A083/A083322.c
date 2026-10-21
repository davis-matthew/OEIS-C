/*
 * A083322  a(n) = 2^n - A081374(n).
 * https://oeis.org/A083322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A083322:
 * a(n) = 2*a(n-1) – a(n-3) + 2*a(n-4)   for n >= 5
 * a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 11
 *
 * The implementation computes terms iteratively in O(n) time,
 * using only a small fixed buffer and no external utilities.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 1) return (seq_t)0;

    /* Handle the first four initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)11;

    /* Buffer of the last four computed terms:
     * t1 = a(k-4), t2 = a(k-3), t3 = a(k-2), t4 = a(k-1) */
    seq_t t1 = (seq_t)1; /* a(1) */
    seq_t t2 = (seq_t)2; /* a(2) */
    seq_t t3 = (seq_t)6; /* a(3) */
    seq_t t4 = (seq_t)11;/* a(4) */

    /* Iteratively compute up to a(n). */
    for (idx_t k = 5; k <= n; ++k) {
        seq_t t5 = (seq_t)2 * t4 - t2 + (seq_t)2 * t1; /* a(k) */
        /* Shift buffer for next iteration: */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
    }

    return t4; /* a(n) stored in t4 after the loop */
}
