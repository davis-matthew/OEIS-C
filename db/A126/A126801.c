/*
 * A126801  a(n) = smallest integer which is coprime to n and is > A057237(n).
 * https://oeis.org/A126801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the smallest integer m > A057237(n) that is coprime to n.
    A057237(n) is the smallest integer k ≥ 1 such that
        gcd(k, n) = 1   and   gcd(k+1, n) > 1.
    Thus, a(n) is the first number after k that is coprime to n.  */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case: n = 1.  None of the conditions for A057237(1) hold,
       but a(1) is defined to be 2. */
    if (n == 1)
        return (seq_t)2;

    /* If n is even, k = 1 always satisfies the conditions:
       gcd(1,n)=1 and 2 divides n so gcd(2,n)>1. */
    if ((n & 1) == 0) {
        seq_t x = 2;                     /* first candidate after k=1 */
        while (gcd((seq_t)n, x) != 1)
            ++x;
        return x;
    }

    /* n is odd.  Find the smallest k with gcd(k,n)=1 and gcd(k+1,n)>1. */
    idx_t k = 1;
    for (;;) {
        if (gcd((seq_t)k, (seq_t)n) == 1 &&
            gcd((seq_t)(k + 1), (seq_t)n) != 1)
            break;
        ++k;
    }

    /* Find the first integer > k that is coprime to n. */
    seq_t x = (seq_t)(k + 1);
    while (gcd((seq_t)n, x) != 1)
        ++x;
    return x;
}
