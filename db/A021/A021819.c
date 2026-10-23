/*
 * A021819  Decimal expansion of 1/815.
 * https://oeis.org/A021819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 815) mod 10 which equals ( (10^(n+1) mod
 *            8150) / 815 ) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute modulo 8150 by binary exponentiation */
#include <stdint.h>

static seq_t modpow10(idx_t exp, idx_t mod)
{
    idx_t result = 1;
    idx_t base = 10 % mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* 10^(n+1) mod 8150 gives value s */
    idx_t exp = n + 1ULL;
    seq_t s = modpow10(exp, (idx_t)8150);
    /* last digit of floor(10^(n+1)/815) is floor(s/815) */
    return (seq_t)((s / 815ULL) % 10ULL);
}
