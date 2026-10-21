/*
 * A066259  a(n) = Fibonacci(n)*Fibonacci(n+1)^2.
 * https://oeis.org/A066259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 4, a(3) = 18, ... */

/* The sequence is computed using fast doubling for Fibonacci numbers.
   Complexity: O(log n) per call.  No memoisation or tables are
   required for n up to 10000 and beyond. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute (F(n), F(n+1)) using fast doubling */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;          /* a = F(k), b = F(k+1) */
    fib_pair(n >> 1, &a, &b);   /* k = floor(n/2) */

    seq_t c = a * ( (b << 1) - a );          /* F(2k)    = F(k) * [2*F(k+1) − F(k)] */
    seq_t d = a * a + b * b;                 /* F(2k+1)  = F(k)^2 + F(k+1)^2 */

    if ( (n & 1) == 0 ) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based as in OEIS; 0 or lower values are not used. */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn * fn1 * fn1;
}
