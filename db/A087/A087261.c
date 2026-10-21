/*
 * A087261  a(n) = lcm(4n, A025586(4n)), least common multiple of 4n and the
 *          largest value in 3x+1 iteration list started at 4n.
 * https://oeis.org/A087261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(4n, max_collatz(4n)) where max_collatz(m) is the
 *            largest value in the Collatz trajectory starting from m and
 *            ending at 1
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the Collatz maximum for a starting value m, then
   return the least common multiple of m and that maximum. */
seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n * (seq_t)4;   /* 4n */
    seq_t x   = m;
    seq_t mx  = m;                     /* current maximum */

    /* iterate until 1 is reached */
    while (x != (seq_t)1) {
        /* even: divide by 2 */
        if ((x & (seq_t)1) == (seq_t)0) {
            x >>= 1;
        } else {                       /* odd: 3x + 1 */
            x = x * (seq_t)3 + (seq_t)1;
        }
        /* update maximum */
        if (x > mx) {
            mx = x;
        }
    }

    /* lcm(m, mx) = m / gcd(m, mx) * mx */
    seq_t g   = gcd(m, mx);
    seq_t res = (m / g) * mx;
    return res;
}
