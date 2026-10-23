/*
 * A032873  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(m) >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose decimal digits alternate
 *            up–down as: d_m >= d_{m-1} <= d_{m-2} >= d_{m-3} <=
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true if the decimal representation of x satisfies the
   alternating up–down pattern described above. */
static bool is_alternating(seq_t x) {
    /* Special case: x == 0, treat as single digit (valid). */
    if (x == (seq_t)0)
        return true;

    uint8_t digits[50];
    size_t len = 0;
    seq_t tmp = x;
    const seq_t TEN = (seq_t)10;

    /* Extract digits from least–significant to most–significant. */
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % TEN);
        tmp /= TEN;
    }

    /* Single digit numbers are always valid. */
    if (len <= 1)
        return true;

    /* Check the alternating pattern from most significant to least. */
    /* Most significant digit index: len-1, next: len-2, ...   */
    for (size_t k = 1; k < len; ++k) {
        uint8_t a = digits[len - k];
        uint8_t b = digits[len - k - 1];
        if ((k & 1) == 1) {            /* k odd -> a >= b */
            if (!(a >= b))
                return false;
        } else {                        /* k even -> a <= b */
            if (!(a <= b))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    /* Sequence offset is 1; ignore n < 1. */
    if (n < (idx_t)1)
        return (seq_t)0;

    idx_t count = 0;
    seq_t current = (seq_t)1;
    while (count < n) {
        if (is_alternating(current)) {
            ++count;
            if (count == n)
                return current;
        }
        ++current;
    }
    /* Should never reach here. */
    return (seq_t)0;
}
