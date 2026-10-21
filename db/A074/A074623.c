/*
 * A074623  a(n) = 7^n + 9^n.
 * https://oeis.org/A074623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binpow.h"

/* OEIS A074623: a(n) = 7^n + 9^n */
seq_t a(idx_t n)
{
    return binpow(7, n) + binpow(9, n);
}
