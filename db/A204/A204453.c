/*
 * A204453  Period length 14: [0, 1, 2, 3, 4, 5, 6, 0, 6, 5, 4, 3, 2, 1]
 *          repeated.
 * https://oeis.org/A204453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n (mod 7) if (-1)^⌊n/7⌋ = +1, otherwise (7 - n) (mod 7)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence with period 14: [0,1,2,3,4,5,6,0,6,5,4,3,2,1] */

#include <stdint.h>          /* for uint64_t, if needed */

seq_t a(idx_t n) {
    idx_t m = n % (idx_t)14;            /* position within the period */
    if (m <= (idx_t)6) {
        return (seq_t)m;                /* 0..6 */
    } else if (m == (idx_t)7) {
        return (seq_t)0;                /* the middle 0 */
    } else {
        return (seq_t)((idx_t)14 - m);  /* 8..13 -> 6..1 */
    }
}
