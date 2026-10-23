/*
 * A037399  Numbers k such that every base-6 digit of k is a base-8 digit of
 *          k.
 * https://oeis.org/A037399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that every base‑6 digit of k is a base‑8 digit of
 *            k Computation: brute‑force search
 * Range:     u8 n <= 14, u16 n <= 3600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: true iff all base‑6 digits of x appear somewhere in its
   base‑8 representation */
static bool valid(seq_t x)
{
    uint8_t mask6 = 0;
    seq_t t = x;
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 6);
        mask6 |= 1u << d;
        t /= 6;
    }

    uint8_t mask8 = 0;
    t = x;
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 8);
        mask8 |= 1u << d;
        t /= 8;
    }

    return (mask6 & ~mask8) == 0;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    for (;; ++k) {
        if (valid(k)) {
            ++found;
            if (found == n) {
                return k;
            }
        }
    }
}
