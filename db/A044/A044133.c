/*
 * A044133  Numbers n such that string 5,1 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th positive integer whose base‑6 representation
   contains the substring 5,1, but the preceding integer does not. */
#include <stdbool.h>

static bool has51(seq_t x) {
    unsigned int digits[64];
    int len = 0;
    seq_t temp = x;
    if (temp == 0) return false;
    while (temp != 0) {
        digits[len++] = (unsigned int)(temp % 6);
        temp /= 6;
    }
    for (int i = 1; i < len; ++i) {
        if (digits[i] == 5 && digits[i-1] == 1) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        if (has51(m) && !has51(m - 1)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
