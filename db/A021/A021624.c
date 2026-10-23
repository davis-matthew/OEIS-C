/*
 * A021624  Decimal expansion of 1/620.
 * https://oeis.org/A021624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10^(n+1) / 620) mod 10 ) Equivalent to: a(n) =
 *            floor( (10^(n+1) mod 6200) / 620 )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^(n+1) mod 6200 */
    const uint64_t MOD      = 6200ULL;   /* 6200 = 10 * 620 */
    const uint64_t DIVISOR  = 620ULL;    /* divisor for the digit */

    uint64_t e   = (uint64_t)n + 1;      /* exponent n+1 */
    uint64_t base = 10ULL;
    uint64_t r    = 1ULL;                /* result of modular exponentiation */

    while (e) {
        if (e & 1ULL)                    /* multiply when bit is set */
            r = (r * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1ULL;
    }

    return (seq_t)(r / DIVISOR);         /* digit is floor(r/620) */
}
