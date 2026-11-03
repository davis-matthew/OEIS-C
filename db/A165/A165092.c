/*
 * A165092  Length of preperiodic part of trajectory of n under iteration of
 *          the base-8 Kaprekar map in A165090.
 * https://oeis.org/A165092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = length of preperiod of the trajectory of n under the
 *            base‑8 Kaprekar map (A165090)
 * Range:     u8 n <= 65, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Validated: standard, pass, 121 terms through n = 3817 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Floyd’s cycle‑finding algorithm to compute the preperiod length. */

#include <stdint.h>

static seq_t kaprec_oct(seq_t x)
{
    // Count the eight octal digits of x
    unsigned char cnt[8] = {0};
    if (x == 0) {
        cnt[0] = 1;
    } else {
        seq_t y = x;
        while (y != 0) {
            unsigned int d = (unsigned int)(y % 8);
            cnt[d] ++;
            y /= 8;
        }
    }

    // Build maximum and minimum values
    seq_t max_val = 0, min_val = 0;
    for (int d = 7; d >= 0; d--) {
        for (int i = 0; i < cnt[d]; i++) {
            max_val = max_val * 8 + d;
        }
    }
    for (int d = 0; d < 8; d++) {
        for (int i = 0; i < cnt[d]; i++) {
            min_val = min_val * 8 + d;
        }
    }

    return max_val - min_val;
}

seq_t a(idx_t n)
{
    seq_t x0 = (seq_t)n;

    // Phase 1: Find a repetition
    seq_t tortoise = kaprec_oct(x0);
    seq_t hare = kaprec_oct(kaprec_oct(x0));
    while (tortoise != hare) {
        tortoise = kaprec_oct(tortoise);
        hare = kaprec_oct(kaprec_oct(hare));
    }

    // Phase 2: Find the start of the cycle (mu = preperiod length)
    tortoise = x0;
    seq_t mu = 0;
    while (tortoise != hare) {
        tortoise = kaprec_oct(tortoise);
        hare = kaprec_oct(hare);
        mu++;
    }

    return mu;
}
