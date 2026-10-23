/*
 * A118952  Characteristic function of numbers that can be written as p+2^k,
 *          where p is prime and p less than 2^k (A118957).
 * https://oeis.org/A118952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=1 if there exists k such that n=p+2^k, where p is a prime
 *            and p<2^k; otherwise a(n)=0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n<6 no representation is possible */
    if (n < 6) return (seq_t)0;

    idx_t pow = 2; /* start with 2^1 */
    while (pow <= n - 2) {
        idx_t p = n - pow;
        if (p < pow && is_prime(p))
            return (seq_t)1;

        /* avoid overflow in next shift */
        if (pow > (UINT64_MAX >> 1))
            break;
        pow <<= 1;
    }
    return (seq_t)0;
}
