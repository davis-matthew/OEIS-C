/*
 * A021367  Decimal expansion of 1/363.
 * https://oeis.org/A021367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = digit at position n+1 in the decimal expansion of 1/363.
   It equals floor(10 * (10^n mod 363) / 363). */
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t MD = 363;
    seq_t res  = 1 % MD;          /* 10^0 mod 363 */
    seq_t base = 10 % MD;         /* 10^1 mod 363 */
    idx_t  e   = n;               /* exponent for 10^n */

    /* binary modular exponentiation: res = 10^n mod 363 */
    while (e) {
        if (e & 1ULL) {
            res = (res * base) % MD;
        }
        base = (base * base) % MD;
        e >>= 1ULL;
    }

    seq_t digit = (10 * res) / MD;  /* floor division gives the decimal digit */
    return digit;
}
