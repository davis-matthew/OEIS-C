/*
 * A021806  Decimal expansion of 1/802.
 * https://oeis.org/A021806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/802) % 10 = (10^(n+1) mod 8020) / 802
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const idx_t mod = 8020;          /* 802 * 10 */
    idx_t exp = n + 1;               /* exponent n+1 */
    idx_t base = 10 % mod;
    idx_t r = 1;                     /* current result modulo mod */

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1)
            r = (r * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    /* r is 10^(n+1) mod 8020, so the quotient digit is r/802 */
    return (seq_t)(r / 802);
}
