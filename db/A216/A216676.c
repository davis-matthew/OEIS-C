/*
 * A216676  Digital roots of squares of Fibonacci numbers.
 * https://oeis.org/A216676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(Fibonacci(n)^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_mod9_rec(idx_t n, uint8_t *fn, uint8_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint8_t f, g;
    fib_mod9_rec(n >> 1, &f, &g);
    uint32_t t1 = (2u * g) % 9u;
    uint32_t t2 = (t1 + 9u - f) % 9u;      /* (2*g - f) mod 9 */
    uint32_t c  = (f * t2) % 9u;           /* F(2k)  */
    uint32_t d  = ((uint32_t)f * f + (uint32_t)g * g) % 9u; /* F(2k+1) */
    if ((n & 1u) == 0u) {
        *fn  = (uint8_t)c;
        *fn1 = (uint8_t)d;
    } else {
        *fn  = (uint8_t)d;
        *fn1 = (uint8_t)((c + d) % 9u);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1 – undefined below */

    uint8_t f_mod9, dummy;
    fib_mod9_rec(n, &f_mod9, &dummy);

    uint32_t sq_mod9 = (uint32_t)f_mod9 * f_mod9 % 9u;
    return (seq_t)(sq_mod9 == 0 ? 9 : sq_mod9);
}
