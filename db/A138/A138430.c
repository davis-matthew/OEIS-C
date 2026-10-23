/*
 * A138430  a(n) = (prime(n)^5 - prime(n))/30.
 * https://oeis.org/A138430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n)) / 30
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* offset starts at 1 – a(0) is undefined */
    seq_t p   = (seq_t)get_prime(n);            /* nth prime */
    seq_t p2  = p * p;
    seq_t p3  = p2 * p;
    seq_t p4  = p3 * p;
    seq_t p5  = p4 * p;
    seq_t diff = p5 - p;
    return diff / (30);                      /* exact integer division */
}
