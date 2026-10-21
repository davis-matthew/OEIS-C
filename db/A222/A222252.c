/*
 * A222252  In the number n, replace all (decimal) digits '7' with '8' and
 *          vice versa.
 * https://oeis.org/A222252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: replace decimal digit 7 with 8 and 8 with 7 in the base‑10 representation of n */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Store digits of n from least significant to most significant (max 20 for 64‑bit). */
    unsigned char digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        unsigned d = digits[i - 1];
        if (d == 7) d = 8;
        else if (d == 8) d = 7;
        result = result * 10 + d;
    }
    return result;
}
