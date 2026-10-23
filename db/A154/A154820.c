/*
 * A154820  Numbers whose trajectory under iteration of sum of cubes of digits
 *          eventually turns out to follow the cyclic iteration of 133, 55,
 *          250.
 * https://oeis.org/A154820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer that finally reaches the cycle * {133,
 *            55, 250} under repeated iteration of f(x) = sum of cubes of
 *            digits. * Computed by a linear search over positive integers
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

/* Return true if x eventually reaches the cycle 133 → 55 → 250 under
 * iteration of sum of cubes of decimal digits.  Implements a simple
 * cycle detection using a bounded chain of intermediate values. */
static bool reaches_cycle(seq_t x)
{
    /* Pre‑computed cubes of 0..9. */
    static const uint32_t cube[10] = {
        0, 1, 8, 27, 64, 125, 216, 343, 512, 729
    };

    /* The trajectory never contains more than a few dozen distinct
     * values before either hitting the cycle or looping elsewhere. */
    static const size_t MAX_CHAIN = 128;
    seq_t chain[MAX_CHAIN];
    size_t len = 0;

    while (1) {
        /* Check if we have reached the cycle. */
        if (x == 133 || x == 55 || x == 250)
            return true;

        /* Detect a repeated value – a loop that does not contain the cycle. */
        for (size_t j = 0; j < len; ++j)
            if (chain[j] == x)
                return false;

        if (len >= MAX_CHAIN)         /* safety guard */
            return false;

        chain[len++] = x;

        /* Compute the next value: sum of cubes of digits. */
        uint8_t digits[64];            /* buffer for decimal digits */
        size_t dlen;
        get_digits(x, digits, &dlen);

        seq_t sum = 0;
        for (size_t k = 0; k < dlen; ++k)
            sum += cube[digits[k]];    /* implicit cast to seq_t */

        x = sum;
    }
}

/* Return the nth element of A154820 (OEIS offset 1). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 0;

    while (1) {
        candidate++;                /* next integer to test */
        if (reaches_cycle(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
    }
}
