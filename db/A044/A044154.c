/*
 * A044154  Numbers n such that string 2,2 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: over integers n, find the nth number whose base‑7
   representation contains the substring "22" but the previous integer does not. */
#include <stdbool.h>

static bool has22(seq_t x) {
    seq_t prev = 7;          /* sentinel value not equal to 2 or 7 */
    while (x != 0) {
        seq_t digit = x % 7;
        if (digit == 2 && prev == 2)
            return true;
        prev = digit;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t cnt = 0;
    seq_t v = 1;
    while (cnt < n) {
        if (has22(v) && !has22(v - 1)) {
            ++cnt;
            if (cnt == n)
                return v;
        }
        ++v;
    }
    return 0;   /* never reached */
}
