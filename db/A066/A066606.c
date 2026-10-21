/*
 * A066606  a(n) = 2^n mod n^2.
 * https://oeis.org/A066606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n mod n^2
 * Range:     u8 n <= 21, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to a 64‑bit unsigned integer for intermediate calculations. */
    uint64_t nn = (uint64_t)n;
    uint64_t mod = nn * nn;              /* n^2, fits in 64 bits for n ≤ 10⁴ */
    uint64_t base = 2 % mod;             /* base reduced modulo n² */
    uint64_t result = 1 % mod;           /* 1 modulo n² (always 1 for mod>1) */
    uint64_t exp = nn;

    /* Binary exponentiation using 128‑bit multiplication to avoid overflow. */
    while (exp != 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1ULL;
    }

    return (seq_t)result;                /* cast final result to seq_t */
}
