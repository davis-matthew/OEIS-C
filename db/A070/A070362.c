/*
 * A070362  a(n) = 3^n mod 44.
 * https://oeis.org/A070362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 44
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t mod = (seq_t)44;
    seq_t result = 1;
    seq_t base = 3 % mod;
    idx_t exp = n;

    while (exp)
    {
        if (exp & 1)
            result = (seq_t)((result * base) % mod);
        base = (seq_t)((base * base) % mod);
        exp >>= 1;
    }

    return result;
}
