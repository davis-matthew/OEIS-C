/*
 * A070700  a(n) = n^7 mod 18.
 * https://oeis.org/A070700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 18
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 18ULL;
    uint64_t r = n % MOD;
    uint64_t res = 1;
    for (unsigned i = 0; i < 7; ++i)
        res = (res * r) % MOD;
    return (seq_t)res;
}
