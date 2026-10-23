/*
 * A102368  Smallest k>0 such that n^k + 1 is not prime.
 * https://oeis.org/A102368
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that n^k + 1 is not prime. For odd n,
 *            a(n)=1 because n+1 is even >2. For even n, a(n)=1 if n+1 is
 *            composite, else a(n)=2 if n^2+1 is composite, else a(n)=3 (since
 *            (n+1)|(n^3+1))
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    /* n is >= 2 at call time */
    if ((n & 1ull) != 0ull)          /* odd n */
        return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    if (!is_prime_u64(n64 + 1))      /* check n+1 */
        return (seq_t)1;

    uint64_t n2 = n64 * n64 + 1;      /* n^2 + 1 */
    if (!is_prime_u64(n2))
        return (seq_t)2;

    /* otherwise n^3+1 is composite */
    return (seq_t)3;
}
