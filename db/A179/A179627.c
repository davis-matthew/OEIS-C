/*
 * A179627  a(n) = length of Collatz sequence starting with n-th prime.
 * https://oeis.org/A179627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006666(A000040(n)) + 1 (Collatz length of the n‑th
 *            prime)
 * Range:     u8 n <= 10, u16 n <= 1614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Prime data type conversion: get_prime returns uint32_t */
    seq_t x   = (seq_t)get_prime(n);   /* n‑th prime (n >= 1) */
    seq_t len = 1;                     /* count includes the starting prime */

    /* Collatz (Syracuse) iteration until 1 is reached */
    while (x != (seq_t)1) {
        if (x & 1)                       /* x is odd: (3*x+1)/2 */
            x = (3 * x + 1) >> 1;
        else                             /* x is even: x/2 */
            x = x >> 1;
        ++len;
    }
    return len;
}
