/*
 * A088950  Number of square-subwords in ternary representation of n.
 * https://oeis.org/A088950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of square‑subwords in the base‑3 representation of
 *            n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A square‑subword is a concatenation XX where X is a non‑empty string of base‑3 digits */

#include <stdbool.h>        /* for bool, true, false */

seq_t a(idx_t n)
{
    /* convert n to base‑3 digits (most significant first) */
    uint8_t digits[64];
    idx_t L = 0;

    if (n == 0) {
        digits[0] = 0;
        L = 1;
    } else {
        idx_t x = n;
        while (x > 0) {
            digits[L++] = (uint8_t)(x % 3);
            x /= 3;
        }
        /* reverse to get most significant first */
        for (idx_t i = 0, j = L - 1; i < j; ++i, --j) {
            uint8_t t = digits[i];
            digits[i] = digits[j];
            digits[j] = t;
        }
    }

    /* count square subwords */
    seq_t count = 0;
    for (idx_t i = 0; i + 1 < L; ++i) {
        idx_t maxK = (L - i) >> 1;          /* floor((L-i)/2) */
        for (idx_t k = 1; k <= maxK; ++k) {
            bool eq = true;
            for (idx_t j = 0; j < k; ++j) {
                if (digits[i + j] != digits[i + k + j]) {
                    eq = false;
                    break;
                }
            }
            if (eq)
                ++count;
        }
    }
    return count;
}
