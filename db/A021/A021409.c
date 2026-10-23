/*
 * A021409  Decimal expansion of 1/405.
 * https://oeis.org/A021409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (floor(10^(n+1) / 405)) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         digits of 1/405 in base 10                 */
#include <stdint.h>

static uint64_t mod_pow10(idx_t exp, uint64_t mod)
{
    /* Compute 10^exp mod mod (exp fits in idx_t) */
    uint64_t result = 1 % mod;
    uint64_t base   = 10 % mod;
    idx_t e = exp;
    while (e) {
        if (e & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^n mod 405 and 10^n mod 4050 */
    uint64_t r405   = mod_pow10(n, 405);
    uint64_t r4050  = mod_pow10(n, 4050);

    /* 10^(n+1) mod 405 and 10^(n+1) mod 4050 */
    uint64_t r4051  = (r405 * 10) % 405;       /* = 10^(n+1) mod 405   */
    uint64_t r40501 = (r4050 * 10) % 4050;     /* = 10^(n+1) mod 4050  */

    /* digit = ((10^(n+1) mod 4050) - (10^(n+1) mod 405)) / 405   */
    uint64_t diff   = r40501 - r4051;           /* multiple of 405      */
    uint64_t digit  = diff / 405;               /* 0 <= digit <= 9     */

    return (seq_t)digit;
}
