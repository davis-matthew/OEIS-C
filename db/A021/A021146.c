/*
 * A021146  Decimal expansion of 1/142.
 * https://oeis.org/A021146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 142) mod 10 the n‑th decimal digit of
 *            1/142
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long MOD = 1420ULL;
    const unsigned long long DEN = 142ULL;
    unsigned long long pow = 1ULL;
    unsigned long long base = 10ULL;
    idx_t exp = n + 1ULL;
    while (exp)
    {
        if (exp & 1ULL)
            pow = (pow * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    unsigned long long digit = pow / DEN;
    return (seq_t)digit;
}
