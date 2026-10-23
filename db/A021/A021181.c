/*
 * A021181  Decimal expansion of 1/177.
 * https://oeis.org/A021181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^{n+1}/177) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th decimal digit of 1/177 (0‑based). */

#include <stdint.h>

static inline uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1) {
            result = (unsigned __int128)result * b % mod;
        }
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^n mod 177  */
    uint64_t rem = powmod_uint64(10U, n, 177U);
    /* digit = (10 * rem) / 177  */
    uint64_t d = (10U * rem) / 177U;
    return (seq_t)d;
}
