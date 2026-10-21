/*
 * A076342  a(n) = A076340(A000040(n)), real part of primes mapped as defined
 *          in A076340, A076341.
 * https://oeis.org/A076342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p + (-1)^{(p+1)/2}, where p = A000040(n) (nth prime)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* nth prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* For p = 2 the real part is 2 */
    if (p == (seq_t)2)
        return p;

    /* For odd primes: 
       if p ≡ 1 (mod 4) -> a = p - 1
       if p ≡ 3 (mod 4) -> a = p + 1
    */
    if ((p & (seq_t)3) == (seq_t)1)
        return p - (seq_t)1;
    else
        return p + (seq_t)1;
}
