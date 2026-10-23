/*
 * A044364  Numbers n such that string 3,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044364
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

/* Sequence: numbers n such that the decimal string "32" occurs in n but not in n-1. */

#include <stdbool.h>

static bool has32(seq_t x)
{
    seq_t prev = 0;
    while (x > 0) {
        seq_t d = x % 10;
        if (prev == 2 && d == 3) {
            return true;
        }
        prev = d;
        x = x / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t number = 0;
    for (;;) {
        ++number;
        if (has32(number) && !has32(number - 1)) {
            ++count;
            if (count == n) {
                return number;
            }
        }
    }
}
