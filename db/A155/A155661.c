/*
 * A155661  a(n) = 10^n - 7^n + 1.
 * https://oeis.org/A155661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 7^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t ten   = (seq_t)10;
    seq_t seven = (seq_t)7;
    seq_t ten_pow   = ipow(ten,   (seq_t)n);
    seq_t seven_pow = ipow(seven, (seq_t)n);
    seq_t result = ten_pow - seven_pow + (seq_t)1;
    return result;
}
