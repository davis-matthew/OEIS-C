/*
 * A087259  a(n) = lcm(n, A025586(n)), least common multiple of n and largest
 *          value in 3x+1 iteration list started at n.
 * https://oeis.org/A087259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087259: a(n) = lcm( n , A025586(n) )
   where A025586(n) is the maximum value reached in the 3x+1 trajectory
   starting at n.  The function simulates the Collatz sequence,
   records its greatest term, then returns the least common multiple
   of the starting value n and that maximum.                                     */
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Euclidean gcd for values of type seq_t.                                    */
/* ------------------------------------------------------------------------- */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------------- */
/* Main function required by the kernel.                                      */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;        /* current value in the 3x+1 trajectory */
    seq_t max = cur;             /* running maximum, start with n */

    while (cur > 1)
    {
        if (cur & 1)                /* odd: 3*x + 1 */
            cur = cur * 3 + 1;
        else                         /* even: x / 2 */
            cur = cur >> 1;
        if (cur > max)
            max = cur;
    }

    /* lcm(n, max) = n / gcd(n, max) * max */
    seq_t g = gcd_seq_t((seq_t)n, max);
    return ((seq_t)n / g) * max;
}
