/*
 * A092293  a(n) = 3*n + A090239(n).
 * https://oeis.org/A092293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n + A090239(n), where A090239(0) = 2 and A090239(n) =
 *            (n + A090239(floor(n/3))) mod 3
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A090239_rec(idx_t n)
{
    if (n == 0) return 2;
    return (unsigned)(((n % 3) + A090239_rec(n / 3)) % 3);
}

seq_t a(idx_t n)
{
    return (seq_t)3 * (seq_t)n + (seq_t)A090239_rec(n);
}
