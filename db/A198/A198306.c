/*
 * A198306  Moore lower bound on the order of a (6,g)-cage.
 * https://oeis.org/A198306
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198306 – Moore lower bound on the order of a (6,g)-cage.
   Formula:
       a(n) = (5^(n/2) - 1) / 2              if n is even (n ≥ 4)
       a(n) = (3 * 5^((n-1)/2) - 1) / 2      if n is odd  (n ≥ 3)
   (The sequence is defined for n ≥ 3; for n < 3 the function returns 0.)
   We compute the required power of 5 using the provided helper
   `big_pow(seq_t base, seq_t exp)`. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                    /* undefined part of the sequence */

    const seq_t FIVE = 5;

    seq_t pow5;
    seq_t result;

    if ((n & 1) == 0) {
        /* even n: a(n) = (5^(n/2) - 1) / 2 */
        pow5 = big_pow(FIVE, (seq_t)(n / 2));
        result = (pow5 - 1) / 2;
    } else {
        /* odd n: a(n) = (3 * 5^((n-1)/2) - 1) / 2 */
        pow5 = big_pow(FIVE, (seq_t)((n - 1) / 2));
        result = (3 * pow5 - 1) / 2;
    }

    return result;
}
