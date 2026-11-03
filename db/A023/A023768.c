/*
 * A023768  Nialpdromes: digits in base 13 are in nonincreasing order.
 * https://oeis.org/A023768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 932 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth nonnegative integer whose base‑13 digits are in
 * nonincreasing order (Nialpdromes).  The function searches
 * sequentially for the n‑th such number. */
#include <stdbool.h>

static inline bool is_nialpdrome(seq_t val)
{
    if (val == 0) return true;
    uint8_t prev_digit = 0;    /* will be overwritten by first digit */
    bool first = true;
    while (val > 0) {
        uint8_t digit = (uint8_t)(val % 13);
        val /= 13;
        if (first) {
            prev_digit = digit;
            first = false;
        } else {
            if (digit < prev_digit)   /* more significant digit is smaller */
                return false;
            prev_digit = digit;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (1) {
        if (is_nialpdrome(x)) {
            count++;
            if (count == n) return x;
        }
        x++;
    }
}
