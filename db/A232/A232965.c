/*
 * A232965  Number of circular n-bit strings that, when circularly shifted by
 *          3 bits, do not have coincident 1's in any position.
 * https://oeis.org/A232965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000032(n/gcd(n,3))^gcd(n,3) (Lucas(n/g)^g)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Fast doubling algorithm for Fibonacci numbers.
   After call, *fn  = F(n), *fn1 = F(n+1)                          */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)              */
    seq_t c = a * ((b << 1) - a);      /* F(2k)                            */
    seq_t d = a * a + b * b;           /* F(2k+1)                          */
    if (n & 1) {
        *fn  = d;                      /* F(2k+1)                          */
        *fn1 = c + d;                  /* F(2k+2)                          */
    } else {
        *fn  = c;                      /* F(2k)                            */
        *fn1 = d;                      /* F(2k+1)                          */
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    const idx_t g = (n % 3 == 0) ? 3 : 1;
    const idx_t m = n / g;

    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);            /* fn = F(m), fn1 = F(m+1)          */

    /* Lucas number L(m) = 2*F(m+1) - F(m)                      */
    seq_t lucas = (fn1 << 1) - fn;

    if (g == 1)
        return lucas;                 /* a(n) = L(m)                       */

    /* a(n) = L(m)^3 for n divisible by 3                       */
    seq_t t = lucas * lucas;
    return t * lucas;
}
