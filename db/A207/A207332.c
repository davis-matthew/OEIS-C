/*
 * A207332  Double factorials (prime(n)-2)!!.
 * https://oeis.org/A207332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0..(prime(n)-3)/2} (2*k+1) = (prime(n)-2)!!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n = 1, prime(1) = 2, and (2-2)!! = 0!! = 1 by convention. */
    uint32_t p = get_prime(n);          /* prime number at index n (>= 2)    */
    seq_t product = (seq_t)1;           /* start with the multiplicative identity */

    /* Multiply all odd numbers from 1 up to p-2.  If p <= 2 the loop is skipped
       and product stays 1, which matches 0!! = 1. */
    for (uint32_t odd = 1; odd <= p - 2; odd += 2)
        product *= (seq_t)odd;

    return product;
}
