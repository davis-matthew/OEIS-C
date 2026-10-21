/*
 * A072815  Sum of proper divisors of 6n + 1.
 * https://oeis.org/A072815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of proper divisors of (6n + 1)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* t = 6n + 1 */
    seq_t t = (seq_t)n * 6 + 1;

    /* For t = 1 (n = 0) the sum of proper divisors is 0. */
    if (t == 1)
        return 0;

    /* Start the sum with 1, the smallest proper divisor of any number > 1. */
    seq_t sum = 1;

    /* Iterate over all possible divisors d >= 2.
       The loop stops when the square of d exceeds t. */
    for (idx_t d = 2;; ++d) {
        seq_t d_seq = (seq_t)d;
        if (d_seq * d_seq > t)
            break;

        if (t % d_seq == 0) {
            sum += d_seq;                     /* add the divisor d */
            seq_t e = t / d_seq;              /* its complementary divisor */
            if (e != d_seq)                  /* if not a perfect square */
                sum += e;                     /* add the complement */
        }
    }

    /* sum now contains all proper divisors of t (1 is already included),
       but it does not include t itself. */
    return sum;
}
