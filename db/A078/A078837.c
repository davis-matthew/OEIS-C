/*
 * A078837  a(n) = Sum_{k=1..prime(n)-1} floor(k^3/prime(n)).
 * https://oeis.org/A078837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n)-2)*(prime(n)-1)*(prime(n)+1))/4, where
 *            prime(n) is the nth prime (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 285, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the nth prime.  */
    seq_t p = nth_prime(n);

    /* Compute (p-2)*(p-1)*(p+1) and divide by 4.  */
    seq_t t1 = p - 2;
    seq_t t2 = p - 1;
    seq_t t3 = p + 1;

    seq_t prod = t1 * t2 * t3;
    return prod / 4;
}
