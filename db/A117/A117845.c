/*
 * A117845  "hello, world" characters encoded in ASCII.
 * https://oeis.org/A117845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, less
 * Formula:   a(n) = (unsigned char)"hello, world"[n mod 12]
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const char s[13] = "hello, world";
    const idx_t len = 12;
    idx_t mod = n % len;
    return (seq_t)(unsigned char)s[(size_t)mod];
}
