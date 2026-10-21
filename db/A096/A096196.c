/*
 * A096196  a(n) = (1 + 2^n) mod n.
 * https://oeis.org/A096196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n) mod n
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the OEIS offset */
    uint64_t mod = (uint64_t)n;
    uint64_t res = 1 % mod;          /* 2^0 */
    uint64_t base = 2 % mod;         /* 2^1 */
    uint64_t e = (uint64_t)n;        /* exponent */

    while (e > 0) {
        if (e & 1U)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1U;
    }

    res = (res + 1U) % mod;          /* (1 + 2^n) mod n */
    return (seq_t)res;
}
