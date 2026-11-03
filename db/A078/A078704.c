/*
 * A078704  Integer part of the square root of phi(n).
 * https://oeis.org/A078704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = floor(sqrt(phi(n)))
 * Range:     u8 n <= 402, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9815 (2026-11-01)
 * Helpers:   isqrt_seq, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t t = totient(n);
    return isqrt_seq(t);
}
