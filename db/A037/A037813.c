/*
 * A037813  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*6^i
 *          is the base-6 representation of n.
 * https://oeis.org/A037813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of i ≥ 1 such that d_i ≤ d_{i-1}, where d_i are
 *            the base‑6 digits of n (least significant first)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute base-6 digits of n.  6^26 ≈ 2.78×10^20 > 2^64-1,
       so at most 27 digits are needed; 32 is safe. */
    unsigned char digits[32];
    unsigned int len = 0;

    idx_t x = n;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 6);
        x /= 6;
    }

    seq_t count = 0;
    for (unsigned int i = 1; i < len; ++i) {
        if (digits[i] <= digits[i - 1]) {
            ++count;
        }
    }
    return count;
}
