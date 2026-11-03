/*
 * A021671  Decimal expansion of 1/667.
 * https://oeis.org/A021671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 if n=0 else floor(10 * (10^n mod 667) / 667) (decimal
 *            digits of 1/667)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t _pow10_mod(idx_t exp)
{
    const uint64_t mod  = 667ULL;
    uint64_t result = 1ULL % mod;
    uint64_t base   = 10ULL;
    uint64_t e      = exp;
    while (e)
    {
        if (e & 1ULL)
            result = (result * base) % mod;
        base   = (base * base) % mod;
        e >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t r = _pow10_mod(n);            /* r = 10^n mod 667  */
    return (seq_t)((10ULL * r) / 667ULL);
}
