/*
 * A084181  a(n) = 2^n+(-2)^n-(-1)^n.
 * https://oeis.org/A084181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + (-2)^n - (-1)^n * Equivalently: a(n) = 1 if n is
 *            odd * 2^(n+1) - 1 if n is even
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* odd indices always give 1 */
    if (n & 1) {
        return (seq_t)1;
    }

    /* n is even: compute 2^(n+1) - 1 */
    idx_t exp = n + 1;        /* exponent for the power of two */
    seq_t val = (seq_t)1;     /* start with 2^0 */

    /* multiply by 2, exp times */
    for (idx_t i = 0; i < exp; ++i) {
        val *= (seq_t)2;
    }

    /* subtract 1 to get 2^(n+1) - 1 */
    val -= (seq_t)1;
    return val;
}
