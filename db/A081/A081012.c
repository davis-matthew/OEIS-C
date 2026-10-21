/*
 * A081012  a(n) = Fibonacci(4n+1) - 2, or Fibonacci(2n+2)*Lucas(2n-1).
 * https://oeis.org/A081012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+1) - 2. Uses fast–doubling Fibonacci
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t fib_fast(idx_t n)
{
    seq_t a = (seq_t)0;   /* F(0) */
    seq_t b = (seq_t)1;   /* F(1) */

    /* Process all 64 bits of the index, most significant bit first. */
    for (int i = 63; i >= 0; --i)
    {
        /* d = F(2k) = a * (2*b - a) */
        seq_t two_b         = b + b;
        seq_t two_b_minus_a = two_b - a;
        seq_t d = a * two_b_minus_a;

        /* e = F(2k+1) = a*a + b*b */
        seq_t e = a * a + b * b;

        if ((n >> i) & (idx_t)1)
        {
            a = e;
            b = d + e;
        }
        else
        {
            a = d;
            b = e;
        }
    }
    return a;   /* F(n) */
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci(4*n+1) and subtract 2. */
    idx_t fib_index = n * (idx_t)4 + (idx_t)1;
    seq_t fn = fib_fast(fib_index);
    return fn - (seq_t)2;
}
