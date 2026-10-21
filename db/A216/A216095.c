/*
 * A216095  a(n) = 2^n mod 10000.
 * https://oeis.org/A216095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = 2^n mod 10000
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t base = 2;
    const seq_t mod = (seq_t)10000;

    while (n > 0)
    {
        if (n & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return res;
}
