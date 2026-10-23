/*
 * A037812  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*5^i
 *          is the base-5 representation of n.
 * https://oeis.org/A037812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of positions i>0 in the base‑5 digits of n * where
 *            digit_i <= digit_(i-1) (i.e., non‑increasing transitions)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    idx_t t = n;
    uint8_t digits[32];
    size_t len = 0;

    while (t > 0) {
        digits[len++] = t % 5;
        t /= 5;
    }

    seq_t cnt = 0;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] <= digits[i - 1]) {
            ++cnt;
        }
    }
    return cnt;
}
