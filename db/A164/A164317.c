/*
 * A164317  Number of binary strings of length n with no substrings equal to
 *          000, 010, or 111.
 * https://oeis.org/A164317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 106, u64 n <= 217,
 *            u128 n <= 439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4, a(3)=5, a(4)=6, a(5)=7, a(6)=9,
   a(7)=11, a(n)=a(n-3)+a(n-4) for n≥8. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial values for n = 0 … 7 */
    static const seq_t init[8] = {1, 2, 4, 5, 6, 7, 9, 11};

    if (n < 8) {
        return init[(size_t)n];
    }

    /* Circular buffer for the last 8 computed values */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = init[i];
    }

    /* Compute a(n) using the recurrence a(k)=a(k-3)+a(k-4) */
    for (idx_t k = 8; k <= n; ++k) {
        size_t r  = (size_t)k & 7;          /* position of a(k) in buf   */
        size_t i3 = (size_t)(k - 3) & 7;    /* position of a(k-3)         */
        size_t i4 = (size_t)(k - 4) & 7;    /* position of a(k-4)         */
        buf[r] = buf[i3] + buf[i4];
    }

    return buf[(size_t)n & 7];
}
