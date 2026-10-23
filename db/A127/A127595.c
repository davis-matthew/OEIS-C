/*
 * A127595  a(n) = F(4n) - 2F(2n) where F(n) = Fibonacci numbers A000045.
 * https://oeis.org/A127595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(4n) - 2F(2n) (F(n) = Fibonacci numbers, F(0)=0,F(1)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------- fast doubling Fibonacci ------------------------------------- */
/* Computes (F(n), F(n+1)) for a given n using recursion.
 * Both outputs are passed by pointer.  Works for any idx_t and seq_t. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                /* a = F(k),  b = F(k+1)  */

    seq_t c = a * ((b << 1) - a);           /* F(2k)   = a*(2b - a)   */
    seq_t d = a * a + b * b;                /* F(2k+1) = a² + b²      */

    if (n & 1U)                             /* n is odd   */
    {
        *fn  = d;                           /* F(2k+1)   */
        *fn1 = c + d;                       /* F(2k+2)   */
    }
    else                                    /* n is even  */
    {
        *fn  = c;                           /* F(2k)     */
        *fn1 = d;                           /* F(2k+1)   */
    }
}

/* ---------- sequence a(n) ----------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t f2, f2_next, f4, f4_next;
    /* F(2n)  */
    fib_pair(2 * n, &f2, &f2_next);
    /* F(4n)  */
    fib_pair(4 * n, &f4, &f4_next);

    seq_t two_f2 = f2 + f2;
    return f4 - two_f2;
}
