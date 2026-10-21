/*
 * A230865  a(n) = n + (sum of digits in base-5 representation of n).
 * https://oeis.org/A230865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + sum of digits in base-5 representation of n
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    idx_t tmp = n;
    while (tmp) {
        result += (seq_t)(tmp % 5);
        tmp /= 5;
    }
    return result;
}
