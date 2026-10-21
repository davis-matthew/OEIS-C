/*
 * A222226  In the number n, replace all (decimal) digits '1' with '9' and
 *          vice versa.
 * https://oeis.org/A222226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace every decimal digit 1 with 9 and every digit 9
 *            with 1 * in the decimal representation of n. For all other
 *            digits, keep them unchanged
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial case n = 0 immediately */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Extract decimal digits of n into an array (least significant first) */
    uint64_t temp = n;
    uint32_t digits[20];   /* 20 digits are enough for a 64‑bit integer */
    int dcount = 0;
    while (temp != 0) {
        digits[dcount++] = (uint32_t)(temp % 10);
        temp /= 10;
    }

    /* Build the result by processing digits from most significant to least */
    seq_t result = 0;
    for (int i = dcount - 1; i >= 0; --i) {
        uint32_t d = digits[i];
        /* Map 1↔9, leave other digits unchanged */
        if (d == 1) d = 9;
        else if (d == 9) d = 1;

        result = result * 10 + (seq_t)d;
    }
    return result;
}
