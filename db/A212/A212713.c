/*
 * A212713  The (decimal equivalent of the) smallest integer that can be made
 *          by rotating the base three digits of n any number of positions to
 *          the left or right.
 * https://oeis.org/A212713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 278, u16 n <= 6559, u32 n <= 6559, u64 n <= 6559,
 *            u128 n <= 6559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = min over all cyclic rotations of the base‑3 representation of n
   interpreted as a base‑3 integer.  The function computes this directly
   by trying every rotation. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; we treat n as the integer value in base‑10 */
    /* Obtain the base‑3 digits of n, most‑significant first */
    int digits[64];
    size_t len = 0;
    idx_t temp = n;
    do {
        digits[len++] = temp % 3;
        temp /= 3;
    } while (temp > 0);

    /* Reverse to get the digits in correct order (most‑significant first) */
    for (size_t i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Evaluate all rotations and keep the smallest decimal value */
    seq_t best = 0;
    bool first = true;

    for (size_t shift = 0; shift < len; ++shift) {
        seq_t val = 0;
        for (size_t j = shift; j < len; ++j) {
            val = val * 3 + (seq_t)digits[j];
        }
        for (size_t j = 0; j < shift; ++j) {
            val = val * 3 + (seq_t)digits[j];
        }
        if (first || val < best) {
            best = val;
            first = false;
        }
    }

    return best;
}
