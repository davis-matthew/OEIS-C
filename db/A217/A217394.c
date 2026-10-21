/*
 * A217394  Numbers starting with 2.
 * https://oeis.org/A217394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (17 * 10^floor(log10(9*n-8)) - 8) / 9
 * Range:     u8 n <= 67, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n) {
    /* n is 1-based; 9*n-8 is guaranteed >= 1 */
    idx_t m = 9 * n - 8;                /* 9*n - 8 */
    int e = digits(m) - 1;             /* floor(log10(9*n-8)) */
    seq_t pow10e = pow10(e);            /* 10^e as seq_t */

    /* Compute the added term: (17*pow10e - 8)/9 */
    seq_t term = (pow10e * (seq_t)17 - (seq_t)8) / (seq_t)9;
    return (seq_t)n + term;
}
