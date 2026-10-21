/*
 * A087272  a(n) is the largest prime number in 3x+1 trajectory initiated at
 *          n.
 * https://oeis.org/A087272
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1818, u32 n <= 7276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = the largest prime number in the Collatz (3x+1) trajectory started at n.
   For n >= 2 this is the correct value; for n < 2 the result is undefined, but 0 is returned. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;         /* undefined for n < 2, but return 0 for safety */
    }

    seq_t maxp = 0;
    seq_t x    = (seq_t)n;

    /* Check the starting value */
    if (is_prime_big(x)) {
        maxp = x;
    }

    /* Collatz sequence: stop when reaching 1 */
    while (x != 1) {
        if ((x & 1u) == 0) {     /* even */
            x = x / 2;
        } else {                 /* odd */
            x = (seq_t)3 * x + 1;
        }

        /* Update the maximum prime seen so far */
        if (is_prime_big(x) && (x > maxp)) {
            maxp = x;
        }
    }

    return maxp;
}
