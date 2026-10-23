/*
 * A036135  a(n) = 2^n mod 83.
 * https://oeis.org/A036135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 83
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t MOD = 83;
    seq_t res  = 1;
    seq_t base = 2;
    idx_t e = n;

    while (e)
    {
        if (e & 1ULL)
            res = (seq_t)((((unsigned long long)res) * base) % MOD);
        base = (seq_t)((((unsigned long long)base) * base) % MOD);
        e >>= 1;
    }
    return res;
}
