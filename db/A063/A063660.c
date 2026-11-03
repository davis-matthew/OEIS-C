/*
 * A063660  Min{ m | m > n and both numbers have at least one common decimal
 *          digit}.
 * https://oeis.org/A063660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the smallest m > n such that n and m have at least one
 *            common decimal digit. The implementation scans forward from n+1
 *            until a match is found
 * Range:     u8 n <= 254, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 85 terms through n = 1886 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether two seq_t numbers share a decimal digit.
   Allowed operations: + - * / % , comparison, ++ --. */
static bool commonDigit(seq_t a, seq_t b)
{
    /* Record which digits appear in a. */
    bool digits[10] = {false};
    seq_t x = a;
    if (x == 0) {
        digits[0] = true;
    } else {
        while (x > 0) {
            int d = (int)(x % 10);
            digits[d] = true;
            x /= 10;
        }
    }

    seq_t y = b;
    if (y == 0) {
        return digits[0];
    }
    while (y > 0) {
        int d = (int)(y % 10);
        if (digits[d]) return true;
        y /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;
    seq_t m = num + 1;
    while (!commonDigit(num, m)) {
        m++;
    }
    return m;
}
