/*
 * A213217  Earliest string in {1,2,...9}* that is order-isomorphic to string
 *          formed by digits of n.
 * https://oeis.org/A213217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A213217: the earliest word in {1,2,…,9}* order‑isomorphic to the
   decimal digit string of n.  Each distinct digit of n is mapped to its rank
   among the distinct digits sorted ascending, producing a string without 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first). */
    uint8_t digits[20];
    size_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Determine which digits appear. */
    unsigned mask = 0;
    for (size_t i = 0; i < len; ++i) {
        mask |= (1u << digits[i]);
    }

    /* Assign ranks to distinct digits in ascending order. */
    uint8_t map[10] = {0};
    unsigned rank = 1;
    for (int d = 0; d <= 9; ++d) {
        if (mask & (1u << d)) {
            map[d] = (uint8_t)rank++;
        }
    }

    /* Reconstruct the resulting number from most significant to least. */
    seq_t result = 0;
    for (size_t k = 0; k < len; ++k) {
        result = result * 10 + map[digits[len - 1 - k]];
    }

    return result;
}
