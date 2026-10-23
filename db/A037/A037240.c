/*
 * A037240  Molien series for 3-D group X1.
 * https://oeis.org/A037240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+5,5)/6 + (n+4)*(n+2)/16 if n≡0,2,4 (mod 6) + (n+3)/9
 *            if n≡0,3 (mod 6) Equivalently, a(n) = ( 24*C(n+5,5) +
 *            9*(n+4)*(n+2) * [n≡0,2,4 (mod 6)] + 16*(n+3) * [n≡0,3 (mod 6)] )
 *            / 144
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 310, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute binomial C(n+5,5) safely using 128‑bit arithmetic */
    __uint128_t prod = (__uint128_t)(n + 5) *
                       (n + 4) *
                       (n + 3) *
                       (n + 2) *
                       (n + 1);
    seq_t binom = (seq_t)(prod / 120);    /* C(n+5,5) fits in seq_t */

    /* build numerator with common denominator 144 */
    __uint128_t numer = (__uint128_t)binom * 24;   /* 24/144 = 1/6 */

    unsigned int r = (unsigned int)(n % 6);
    if (r == 0 || r == 2 || r == 4) {
        numer += (__uint128_t)(n + 4) * (n + 2) * 9;   /* +9/144 = 1/16 */
    }
    if (r == 0 || r == 3) {
        numer += (__uint128_t)(n + 3) * 16;           /* +16/144 = 1/9 */
    }

    return (seq_t)(numer / 144);     /* exact integer division */
}
