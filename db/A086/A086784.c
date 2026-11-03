/*
 * A086784  Number of non-trailing zeros in binary representation of n.
 * https://oeis.org/A086784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitlen(n) - popcount(n) - v2(n) (n>0, a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9823 (2026-11-01)
 * Helpers:   bit_length_u64, ctz_u64, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length_u64.h"
#include "../utils/ctz_u64.h"
#include "../utils/popcount.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t x = (uint64_t)n;

    int trailing = ctz_u64(x);          /* v2(n) */
    int bits     = bit_length_u64(x);      /* bitlen(n) */
    int ones     = popcount(x);    /* popcount(n) */

    int nontrailing_zeros = bits - ones - trailing;
    return (seq_t)nontrailing_zeros;
}
