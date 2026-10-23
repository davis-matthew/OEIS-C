/*
 * A039635  Fixed point of "n -> n/2 or (n+1)/2 until result is prime".
 * https://oeis.org/A039635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed‑point of f(n) = n/2 if n even, (n+1)/2 if n odd,  
   until the result is prime.  For n = 1 the function returns 1. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 1)                 /* 1 is the fixed point of the map */
        return (seq_t)1;

    seq_t cur = (seq_t)n;
    while (!is_prime_big(cur))  /* loop until a prime is reached */
    {
        if ((cur & (seq_t)1) == 0)     /* even → divide by 2 */
            cur >>= 1;
        else                          /* odd  → (n+1)/2 */
            cur = (cur + 1) >> 1;
    }
    return cur;
}
