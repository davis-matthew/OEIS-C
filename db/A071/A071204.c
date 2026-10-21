/*
 * A071204  Numbers which are multiples of their largest digit (decimal
 *          notation).
 * https://oeis.org/A071204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A071204 – Numbers which are multiples of their largest decimal digit.
   The sequence is obtained by simple search: iterate the natural numbers,
   test each one and keep the count of valid terms until the nth term is found.
   This works for all integer types (uint8_t … uint64_t, unsigned __int128
   and for any arbitrary‑precision integer class that supports + - * / % and
   comparison operators).
*/

#include <stdbool.h>

static inline seq_t largest_digit(seq_t v) {
    seq_t md = 0;
    while (v > 0) {
        seq_t d = v % 10;
        if (d > md) md = d;
        v /= 10;
    }
    return md;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, a(0) is unused */
    idx_t count = 0;
    seq_t x = 1;

    while (true) {
        seq_t d = largest_digit(x);
        if (d != 0 && x % d == 0) {
            count++;
            if (count == n) return x;
        }
        x++;
    }
}
