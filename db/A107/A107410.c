/*
 * A107410  Each term is sum of three previous terms mod 9.
 * https://oeis.org/A107410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1) + a(n-2) + a(n-3)) mod 9; Initial terms: a(0) =
 *            0, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Periodic sequence of length 39 */
    static seq_t period[39];
    static bool initialized = false;
    if (!initialized) {
        period[0] = (seq_t)0;
        period[1] = (seq_t)1;
        period[2] = (seq_t)2;
        for (idx_t i = 3; i < 39; ++i) {
            period[i] = (seq_t)((period[i-1] + period[i-2] + period[i-3]) % 9);
        }
        initialized = true;
    }
    idx_t idx = n % 39;
    return period[idx];
}
