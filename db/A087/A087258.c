/*
 * A087258  a(n) = gcd(n, A025586(n)), greatest common divisor of n and
 *          largest value in 3x+1 iteration list started at n.
 * https://oeis.org/A087258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, max value in the Collatz sequence starting at n)
 * Range:     u8 n <= 54, u16 n <= 9898, u32 n <= 9898, u64 n <= 9898,
 *            u128 n <= 9898, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the nth term of A087258 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1; 0 is undefined but handled defensively */

    seq_t sn = (seq_t)n;          /* n as a seq_t for arithmetic */
    seq_t x  = sn;                /* current value in the 3x+1 iteration */
    seq_t mx = x;                 /* largest value seen so far */

    while (x != (seq_t)1) {
        if ((x & (seq_t)1) == 0)  /* even */
            x >>= 1;             /* x = x / 2 */
        else                      /* odd */
            x = x * (seq_t)3 + (seq_t)1;

        if (x > mx)
            mx = x;
    }

    return gcd(sn, mx);          /* gcd of the starting value and the maximum observed */
}
