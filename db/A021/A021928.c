/*
 * A021928  Decimal expansion of 1/924.
 * https://oeis.org/A021928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/924) mod 10
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = floor(10 * (10^n mod 924) / 924) */
#include <stdint.h>

static seq_t pow10_mod924(idx_t n)
{
    /* Compute 10^n mod 924 by binary exponentiation */
    uint64_t base = 10;          /* 10 < 924 */
    uint64_t mod  = 924;
    uint64_t res  = 1 % mod;      /* 10^0 == 1 */
    idx_t exp = n;

    while (exp > 0)
    {
        if (exp & 1ULL)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)res;           /* 0 <= res < 924 */
}

seq_t a(idx_t n)
{
    /* r = 10^n mod 924 */
    seq_t r = pow10_mod924(n);
    /* digit = floor(10 * r / 924) */
    return (seq_t)((10ULL * (uint64_t)r) / 924ULL);
}
