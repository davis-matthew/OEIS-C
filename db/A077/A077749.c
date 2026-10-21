/*
 * A077749  a(n) contains 2^n digits such that the concatenation
 *          a(0)a(1)a(2)...forms a substring of the cyclic concatenation
 *          012345678901234567890123...
 * https://oeis.org/A077749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = integer formed by concatenating 2^n consecutive digits of
 *            the infinite periodic string "0123456789" starting at offset 2^n
 *            − 1. That is, if L = 2^n and s = L − 1, then a(n) =
 *            ∑_{k=0}^{L−1} ((s + k) mod 10) · 10^{L−1−k}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* length = 2^n */
    seq_t len = 1;
    for (idx_t i = 0; i < n; ++i) {
        len *= 2;
    }
    /* starting position s = len-1 */
    seq_t start = len - 1;
    seq_t result = 0;
    seq_t i = 0;
    while (i < len) {
        seq_t digit = (start + i) % 10;
        result = result * 10 + digit;
        ++i;
    }
    return result;
}
