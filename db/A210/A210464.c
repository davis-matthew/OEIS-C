/*
 * A210464  Number of bracelets with 2 blue, 2 red, and n black beads.
 * https://oeis.org/A210464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is odd: ((n-1)/2)^3 + 5*((n-1)/2)^2 + 8*((n-1)/2) +
 *            4 if n ≡ 0 (mod 4): (n/4) * (8*(n/4)^2 + 14*(n/4) + 9) + 2 if n
 *            ≡ 2 (mod 4): ((n-2)/4) * (8*((n-2)/4)^2 + 26*((n-2)/4) + 29) +
 *            11
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n >= 1 */
    seq_t res;
    if (n & 1U) { /* odd n: 2m+1 */
        seq_t m = (seq_t)(n >> 1);          /* m = (n-1)/2 */
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        res = m3 + 5 * m2 + 8 * m + 4;
    } else if ((n & 3U) == 0) {   /* n ≡ 0 (mod 4) */
        seq_t m = (seq_t)(n >> 2);          /* m = n/4 */
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        res = 8 * m3 + 14 * m2 + 9 * m + 2;
    } else { /* n ≡ 2 (mod 4) */
        seq_t m = (seq_t)((n - 2) >> 2);    /* m = (n-2)/4 */
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        res = 8 * m3 + 26 * m2 + 29 * m + 11;
    }
    return res;
}
