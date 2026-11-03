/*
 * A033056  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A033056: numbers whose base‑5 representation has odd digits at
 *  odd positions (counted from the most significant digit).   */

#include <stdbool.h>
#include <stddef.h>

/* Return true if v satisfies the property. */
static bool valid_base5(seq_t v)
{
    if (v == 0)               /* 0 is not in the sequence      */
        return false;

    unsigned int digits[64];
    idx_t len = 0;
    seq_t tmp = v;

    while (tmp) {
        digits[(size_t)len++] = (unsigned int)(tmp % 5);
        tmp /= 5;
    }

    /* Digits are stored LSB→MSB in digits[0..len-1].
       We need to examine them from MSB→LSB (position 0 is MSB). */
    for (idx_t pos = 0; pos < len; ++pos) {
        unsigned int d = digits[(size_t)(len - 1 - pos)];
        if ((pos & 1u) && ((d & 1u) == 0))   /* odd position must be odd digit */
            return false;
    }
    return true;
}

/* Return the nth term of the sequence, where n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t val   = 0;   /* current candidate */
    idx_t cnt   = 0;   /* how many valid numbers found */

    while (cnt < n) {
        ++val;
        if (valid_base5(val))
            ++cnt;
    }
    return val;
}
