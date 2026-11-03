/*
 * A081667  a(n) = Fibonacci(binomial(n+2,2)).
 * https://oeis.org/A081667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(binomial(n+2,2))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 94 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute F(n) and F(n+1) on the fly using fast doubling.
 * Works for any seq_t that supports +, -, *, <<, and =. */
static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair_rec(n >> 1, &a, &b);               /* a = F(k), b = F(k+1), k = n/2 */

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    /* d = F(2k+1) = F(k)*F(k) + F(k+1)*F(k+1) */
    c = a * ((b << 1) - a);
    d = a * a + b * b;

    if (n & 1) { /* n is odd: return F(2k+1), F(2k+2) = F(2k)+F(2k+1) */
        *fn = d;
        *fn1 = c + d;
    } else {     /* n is even: return F(2k), F(2k+1) */
        *fn = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    /* binomial(n+2,2) = (n+2)(n+1)/2 */
    idx_t k = ((unsigned __int128)(n + 2) * (n + 1)) / 2;

    seq_t fn, fn1;
    fib_pair_rec(k, &fn, &fn1);
    return fn;
}
