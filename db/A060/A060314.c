/*
 * A060314  a(n) = n + next number > n beginning with same digit as n.
 * https://oeis.org/A060314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n + next number > n that begins with the same first digit
 *            as n
 * Range:     u8 n <= 38, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   next_same
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/next_same.h"

seq_t a(idx_t n) {
    seq_t nxt = next_same(n);
    return (seq_t)n + nxt;
}
