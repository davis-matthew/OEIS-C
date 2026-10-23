/*
 * A044374  Numbers n such that string 4,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that contains the decimal substring "42"
   but whose predecessor does not contain that substring. */
/* This implementation scans numbers sequentially up to the requested
   index; it is efficient enough for n up to about 10 000. */
#include <stdbool.h>

static inline bool has_42(seq_t x) {
    int prev = -1;             /* previous (less significant) digit */
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 4 && prev == 2)   /* more significant digit 4, lower digit 2 */
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    while (count < n) {
        m++;
        if (has_42(m) && !has_42(m - 1))
            count++;
    }
    return m;
}
