/*
 * A037882  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*5^i} is the base 5
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Sum_{i} |d_i - e_i| , where Sum{d_i*5^i} is the
 *            base‑5 representation of n and e_i are the digits d_i sorted in
 *            nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be >= 1 in the OEIS indexing; return 0 if n == 0. */
    if (n == 0)
        return 0;

    /* Extract base‑5 digits of n (least significant first). */
    unsigned char digits[32];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 5;
        tmp /= 5;
    }

    /* Sort the digits into ascending order.  Insertion sort is
       sufficient for the at most 28 digits that occur for 64‑bit n. */
    unsigned char sorted[32];
    for (size_t i = 0; i < len; ++i)
        sorted[i] = digits[i];

    for (size_t i = 1; i < len; ++i) {
        unsigned char key = sorted[i];
        size_t j = i;
        while (j > 0 && sorted[j - 1] > key) {
            sorted[j] = sorted[j - 1];
            --j;
        }
        sorted[j] = key;
    }

    /* Compute the sum of absolute differences. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char di = digits[i];
        unsigned char ei = sorted[i];
        sum += (di > ei) ? (seq_t)(di - ei) : (seq_t)(ei - di);
    }

    /* The sum is always even; return half of it. */
    return sum / 2;
}
