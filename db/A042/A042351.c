/*
 * A042351  Denominators of continued fraction convergents to sqrt(702).
 * https://oeis.org/A042351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042351: Denominators of convergents to √702.
   Recurrence: a(0)=1, a(1)=2, a(2)=105, a(3)=212,
                a(n) = 106*a(n-2) - a(n-4)   for n > 3
*/
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle base values directly */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 105;
    if (n == 3) return 212;

    /* Circular buffer holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1;   /* a(n-4) */
    seq_t a1 = 2;   /* a(n-3) */
    seq_t a2 = 105; /* a(n-2) */
    seq_t a3 = 212; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = 106ULL * a2 - a0; /* a(n) = 106*a(n-2) - a(n-4) */
        /* Shift indices for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3; /* a(n) is stored in a3 after the loop */
}
