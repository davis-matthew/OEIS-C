/*
 * A168427  a(n) = 3^n mod 30.
 * https://oeis.org/A168427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 30
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast binary exponentiation modulo 30 */
    const seq_t MOD = (seq_t)30;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & (idx_t)1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}
