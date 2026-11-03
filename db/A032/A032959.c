/*
 * A032959  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9740 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032959: Nth integer whose base‑9 digits at odd positions (from most
   significant) are even. */
#include <stdbool.h>

static inline bool a032959_valid(seq_t x)
{
    seq_t tmp = x;
    int digits[64];
    int len = 0;

    /* extract digits in reverse order (least significant first) */
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 9);
        tmp /= 9;
    }
    if (len == 0)   /* handle x == 0 – never reached in this sequence */
        digits[len++] = 0;

    /* check from most significant digit (pos=0) */
    for (int pos = 0; pos < len; ++pos) {
        int d = digits[len - 1 - pos];
        if ((pos & 1) && (d & 1))   /* odd position and digit is odd */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t count = 0;

    while (1) {
        if (a032959_valid(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
