/*
 * A021340  Decimal expansion of 1/336.
 * https://oeis.org/A021340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point in 1/336. It is
 *            obtained by computing the remainder r = 10^n mod 336, then a(n)
 *            = (r * 10) / 336
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long modpow_uint(unsigned long long base, idx_t exp, unsigned long long mod) {
    unsigned long long result = 1ULL % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    const unsigned long long MOD = 336ULL;
    unsigned long long r = modpow_uint(10ULL, n, MOD);
    seq_t digit = (seq_t)((r * 10ULL) / MOD);
    return digit;
}
