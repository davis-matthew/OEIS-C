/*
 * A060761  a(n) = 9^(n^2).
 * https://oeis.org/A060761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t base = (seq_t)9;    /* base 9 */
    seq_t exp  = (seq_t)n * (seq_t)n;  /* exponent n^2 */
    seq_t result = (seq_t)1;          /* 9^0 = 1 */

    while (exp > (seq_t)0) {
        if (exp & (seq_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}
