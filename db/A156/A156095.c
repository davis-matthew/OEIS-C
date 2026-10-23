/*
 * A156095  5 F(2n) (F(2n) + 1) + 1 where F(n) denotes the n-th Fibonacci
 *          number.
 * https://oeis.org/A156095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5·F(2n)·(F(2n)+1)+1, where F is Fibonacci with F(0)=0,
 *            F(1)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: returns F(n) for 0 ≤ n.  Uses only integer ops. */
static seq_t fib_fast(idx_t n)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* Find the highest set bit in n */
    int bit = 63;
    while (bit >= 0 && ((n >> bit) & 1ull) == 0)
        --bit;

    for (; bit >= 0; --bit)
    {
        /* Compute: d = F(2k), e = F(2k+1) where a=F(k), b=F(k+1) */
        seq_t two_b = b + b;
        seq_t d = a * (two_b - a);          /* F(2k)  = F(k) * (2*F(k+1) - F(k)) */
        seq_t e = a * a + b * b;            /* F(2k+1)= F(k)^2 + F(k+1)^2 */

        if (((n >> bit) & 1ull) == 0)
        {
            a = d;   /* next k = 2k   */
            b = e;   /* next k+1 = 2k+1 */
        }
        else
        {
            a = e;               /* next k = 2k+1 */
            b = d + e;           /* next k+1 = 2k+2 */
        }
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t m = n << 1;                 /* m = 2n */
    seq_t f = fib_fast(m);            /* F(2n) */
    seq_t t = f + 1;                  /* F(2n)+1 */
    seq_t r = f * t;                  /* F(2n)*(F(2n)+1) */
    r = r * 5;                        /* *5 */
    r = r + 1;                        /* +1 */
    return r;
}
