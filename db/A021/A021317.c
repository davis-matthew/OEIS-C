/*
 * A021317  Decimal expansion of 1/313.
 * https://oeis.org/A021317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^{n+1} / 313) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the (n+1)th digit of the decimal expansion of 1/313. */

#include <stdint.h>

/* modular exponentiation: base^exp mod mod (mod > 0) */
static inline unsigned long long mod_pow(unsigned long long base,
                                         unsigned long long exp,
                                         unsigned long long mod)
{
    unsigned long long result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        exp >>= 1ULL;
        if (exp)
            base = (base * base) % mod;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* We need the remainder of 10^(n+1) modulo 3130 = 313 * 10.
       The desired digit is floor(rem / 313). */
    const unsigned long long MOD = 3130ULL;          /* 313 * 10 */
    const unsigned long long BASE = 10ULL % MOD;     /* 10 */
    unsigned long long rem = mod_pow(BASE, (unsigned long long)(n + 1), MOD);
    unsigned long long digit = rem / 313ULL;        /* 0 <= digit < 10 */
    return (seq_t)digit;
}
