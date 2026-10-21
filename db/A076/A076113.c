/*
 * A076113  a(n) = n^(n*(n-1)/2).
 * https://oeis.org/A076113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n*(n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)1;           /* 0^0 = 1, 1^0 = 1   */
    seq_t exp = (seq_t)n * ((seq_t)n - 1) / 2;        /* exponent = n*(n-1)/2 */
    return pow_seq((seq_t)n, exp);
}
