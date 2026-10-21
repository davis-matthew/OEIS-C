/*
 * A087260  a(n) = gcd(4n, A025586(4n)), greatest common divisor of 4n and
 *          largest value in 3x+1 iteration list started at 4n.
 * https://oeis.org/A087260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(4·n, A025586(4·n))
 * Range:     u8 n <= 26, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025586(x) is the largest value reached in the 3x+1 (Collatz) iteration starting at x.  */
/* The function below iterates the Collatz sequence until 1 is reached, keeps track of the maximum value, */
/* and returns the greatest common divisor of 4n and that maximum. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* According to the OEIS offset, n is a positive integer. */
    if (n == 0) return 0;                /* No defined value for n = 0, return 0 safely. */

    seq_t start = (seq_t)n * (seq_t)4;   /* 4n */
    seq_t current = start;
    seq_t max_val = start;

    /* Perform the Collatz iteration. */
    while (current > 1) {
        if (current % 2 == 0) {
            current /= 2;
        } else {
            current = current * 3 + 1;
        }
        if (current > max_val) {
            max_val = current;
        }
    }

    /* Return gcd(4n, max_val). */
    return gcd(start, max_val);
}
