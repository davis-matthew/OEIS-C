/*
 * A036133  a(n) = 5^n mod 73.
 * https://oeis.org/A036133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 73
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* fast modular exponentiation: 5^n mod 73 */
    const seq_t MOD = (seq_t)73;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    idx_t exp = n;

    while (exp) {
        if (exp & 1U)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1U;
    }
    return result;
}
