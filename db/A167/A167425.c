/*
 * A167425  a(n) = 2^n mod 26.
 * https://oeis.org/A167425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 26
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t MOD = 26;
    seq_t result = 1;
    seq_t base = 2;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U)
            result = (seq_t)((uint64_t)result * base % MOD);
        exp >>= 1;
        if (exp)
            base = (seq_t)((uint64_t)base * base % MOD);
    }
    return result;
}
