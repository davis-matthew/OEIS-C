/*
 * A078838  a(n) = Sum_{k=1..(p-1)*(p-2)} floor((k*p)^(1/3)) where p is the
 *          n-th prime.
 * https://oeis.org/A078838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (1/4)*(3*p-5)*(p-2)*(p-1) where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (the offset is 1, so this value is undefined). */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n-th prime.  get_prime returns a 32‑bit unsigned
       integer which is sufficient for all n we care about (p grows
       slower than n log n). */
    uint32_t p = get_prime(n);

    /* Compute the formula using seq_t arithmetic.  The intermediate
       product fits in seq_t; the division by 4 is exact because the
       expression is always divisible by 4. */
    seq_t t1 = (seq_t)(3 * (seq_t)p - 5);
    seq_t t2 = (seq_t)(p - 2);
    seq_t t3 = (seq_t)(p - 1);
    seq_t prod = t1 * t2 * t3;
    return prod / (seq_t)4;
}
