/*
 * A044922  Base 6 run lengths form a palindrome.
 * https://oeis.org/A044922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose base-6 run-lengths form a palindrome */
#include <stdbool.h>
#include <stddef.h>

static bool base6_runlen_palindrome(idx_t x)
{
    idx_t y = x;
    uint8_t digits[64];
    size_t d = 0;
    /* Extract digits of x in base 6 (least‑significant first) */
    do {
        digits[d++] = (uint8_t)(y % 6);
        y /= 6;
    } while (y);

    /* Build run lengths in order from most significant to least */
    size_t lens[64];
    size_t lens_len = 0;
    size_t i = d;
    while (i > 0) {
        size_t run = 1;
        uint8_t cur = digits[--i];
        while (i > 0 && digits[i-1] == cur) {
            run++;
            i--;
        }
        lens[lens_len++] = run;
    }

    /* Check whether the run‑length list is a palindrome */
    for (size_t j = 0; j < lens_len / 2; ++j) {
        if (lens[j] != lens[lens_len - 1 - j])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num = 0;
    while (count < n) {
        ++num;
        if (base6_runlen_palindrome(num))
            ++count;
    }
    return (seq_t)num;
}
