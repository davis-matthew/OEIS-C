/*
 * A074459  Number of segments which change from display of a number n to the
 *          next number n+1 on a 7-segment display: version where '6', '7',
 *          '9' use 6, 4, resp. 5 segments.
 * https://oeis.org/A074459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 7‑segment changes from n to n+1 using the digit
 *            segment counts of A074458 (6,4,5 for 6,7,9)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   popcnt8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcnt8.h"

/* 7‑segment bit patterns for digits 0‑9 with the variant
   (0=63, 1=6, 2=91, 3=79, 4=102, 5=109, 6=125, 7=71, 8=127, 9=103). */
static const uint8_t seg[10] = {
    63, /* 0 */
    6,  /* 1 */
    91, /* 2 */
    79, /* 3 */
    102,/* 4 */
    109,/* 5 */
    125,/* 6 */
    71, /* 7 */
    127,/* 8 */
    103  /* 9 */
};

seq_t a(idx_t n)
{
    /* n+1 may wrap around for the maximum idx_t, but that case is not needed
       for the tested ranges. */
    idx_t m = n + 1;

    /* Collect decimal digits of n and m in reverse order. */
    unsigned char digits_n[21];
    unsigned char digits_m[21];
    idx_t len_n = 0, len_m = 0;

    /* Handle n = 0 as a single digit 0. */
    if (n == 0) {
        digits_n[0] = 0;
        len_n = 1;
    } else {
        idx_t tmp = n;
        while (tmp) {
            digits_n[len_n++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Handle m = 0 analogously (only possible if n = UINT64_MAX). */
    if (m == 0) {
        digits_m[0] = 0;
        len_m = 1;
    } else {
        idx_t tmp = m;
        while (tmp) {
            digits_m[len_m++] = tmp % 10;
            tmp /= 10;
        }
    }

    idx_t maxlen = (len_n > len_m) ? len_n : len_m;
    seq_t total = 0;

    for (idx_t i = 0; i < maxlen; ++i) {
        uint8_t mask1 = (i < len_n) ? seg[digits_n[i]] : 0;  /* blank => 0 */
        uint8_t mask2 = (i < len_m) ? seg[digits_m[i]] : 0;
        total += (seq_t)popcnt8(mask1 ^ mask2);
    }

    return total;
}
