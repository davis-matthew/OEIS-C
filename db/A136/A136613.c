/*
 * A136613  Concatenation of (sum of digits of n) and n.
 * https://oeis.org/A136613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = concat(digit_sum(n), n)
 * Range:     u8 n <= 11, u16 n <= 1005, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute sum of decimal digits of n. */
    idx_t sum = 0;
    idx_t t = n;
    while (t) {
        sum += t % 10;
        t /= 10;
    }

    /* Compute the number of decimal digits of n (at least 1 for n == 0). */
    idx_t d = 0;
    t = n;
    if (t == 0) {
        d = 1;
    } else {
        while (t) {
            d++;
            t /= 10;
        }
    }

    /* Compute 10^d as a seq_t value. */
    seq_t pow10_d = pow10(d);

    /* Concatenate: (sum × 10^d) + n. */
    return (seq_t)sum * pow10_d + (seq_t)n;
}
