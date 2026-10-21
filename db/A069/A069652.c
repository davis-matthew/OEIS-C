/*
 * A069652  GCD of all the numbers obtained by permuting the digits of n.
 * https://oeis.org/A069652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = GCD of all numbers formed by permuting the decimal digits
 *            of n
 * Range:     u8 n <= 104, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* guard: sequence is defined for n >= 1, but returning 0 for 0 is harmless */
    if (n == 0) return 0;

    /* extract decimal digits of n, least‑significant first */
    uint8_t digits[20];
    idx_t tmp = n;
    int len = 0;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* precompute powers of 10: pow10[i] = 10^i (as seq_t) */
    seq_t pow10[20];
    pow10[0] = 1;
    for (int i = 1; i < len; ++i) {
        pow10[i] = pow10[i - 1] * 10;
    }

    /* start with the number itself */
    seq_t g = (seq_t)n;

    /* accumulate gcd of all pairwise digit‑swap differences */
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            int diff_digit = digits[i] > digits[j] ? digits[i] - digits[j] 
                                                   : digits[j] - digits[i];
            if (diff_digit == 0) continue;                     /* identical digits */
            seq_t diff = (seq_t)diff_digit * (pow10[j] - pow10[i]); /* positive */
            g = gcd(g, diff);
        }
    }

    return g;
}
