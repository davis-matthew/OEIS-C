/*
 * A041962  Numerators of continued fraction convergents to sqrt(504).
 * https://oeis.org/A041962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 898 * a(n-4) - a(n-8);
 * Initial terms (n = 0..7):
 *   a(0)=22, a(1)=45, a(2)=202, a(3)=449,
 *   a(4)=19958, a(5)=40365, a(6)=181418, a(7)=403201
 *
 * This implementation uses only the recurrence and a small fixed buffer
 * of eight values.  It works for arbitrary precision integer types
 * supplied by the build (seq_t).
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for the first eight terms */
    static const seq_t base[8] = {
        22, 45, 202, 449,
        19958, 40365, 181418, 403201
    };

    if (n < 8) {
        return base[n];
    }

    /* Circular buffer holding the last 8 computed terms */
    static seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = base[i];
    }

    /* Compute terms from 8 up to n */
    for (idx_t i = 8; i <= n; ++i) {
        size_t idx4 = (size_t)(i - 4) % 8;
        size_t idx8 = (size_t)(i - 8) % 8;
        seq_t prod = buf[idx4] * (seq_t)898;
        buf[(size_t)i % 8] = prod - buf[idx8];
    }

    return buf[(size_t)n % 8];
}
