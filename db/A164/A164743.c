/*
 * A164743  Digital root of 3*A000045(n).
 * https://oeis.org/A164743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = A010888(3*A000045(n)) (digital root of 3*Fibonacci)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm that returns F(n) and F(n+1) modulo 9. */
static void fib_pair(idx_t n, uint32_t *fn, uint32_t *fn1)
{
    if (n == 0) {
        *fn  = 0;      /* F(0) */
        *fn1 = 1;      /* F(1) */
        return;
    }
    uint32_t a, b;
    fib_pair(n >> 1, &a, &b);                 // a=F(k), b=F(k+1)

    uint32_t c = (a * ((2 * b + 9 - a) % 9)) % 9;   // F(2k)
    uint32_t d = (a * a % 9 + b * b % 9) % 9;       // F(2k+1)

    if ((n & 1) == 0) {                    // n even
        *fn  = c;
        *fn1 = d;
    } else {                               // n odd
        *fn  = d;
        *fn1 = (c + d) % 9;
    }
}

seq_t a(idx_t n)
{
    uint32_t fib_mod9, tmp;
    fib_pair(n, &fib_mod9, &tmp);           // fib_mod9 = F(n) mod 9

    uint32_t m = (3 * fib_mod9) % 9;        // 3*F(n) mod 9
    if (m == 0) m = 9;                      // digital‑root convention

    return (seq_t)m;
}
