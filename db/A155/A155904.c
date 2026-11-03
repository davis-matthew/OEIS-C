/*
 * A155904  Number of ways to write 2n-1 as p+2^x+5*2^y with p an odd prime
 *          and x,y positive integers.
 * https://oeis.org/A155904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of representations of 2n-1 as p + 2^x + 5·2^y
 *            where p is an odd prime and x,y are positive integers
 * Range:     u8 n <= 50000, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 104 terms through n = 8982 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A155904: the number of ways to write 2n - 1 as p + 2^x + 5 2^y with p an odd
 * prime and x, y >= 1, with the exact 64-bit primality test (the original's
 * prime table stopped at 20000, i.e. n = 10000). */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t M = 2 * (uint64_t)n - 1;
    uint64_t count = 0;
    for (unsigned x = 1; x < 63 && (UINT64_C(1) << x) < M; ++x) {
        const uint64_t d = M - (UINT64_C(1) << x);
        for (unsigned y = 1; y < 61 && 5 * (UINT64_C(1) << y) <= d; ++y) {
            uint64_t p = d - 5 * (UINT64_C(1) << y);
            if (p > 2 && (p & 1) && is_prime_u64(p)) ++count;
        }
    }
    return (seq_t)count;
}
