/*
 * A021432  Decimal expansion of 1/428.
 * https://oeis.org/A021432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( 10 * (10^n mod 428) / 428 )
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_mod(idx_t exp, seq_t mod)
{
    seq_t result = 1u;
    seq_t base   = 10u;   // 10 mod 428

    while (exp)
    {
        if (exp & 1u)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 428 */
    const seq_t mod = 428u;
    seq_t r = pow10_mod(n, mod);

    /* Digit = floor((r * 10) / 428) */
    seq_t digit = (r * 10u) / mod;
    return digit;  /* 0 <= digit <= 9 */
}
