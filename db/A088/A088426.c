/*
 * A088426  Number of primes in arithmetic progression starting with 19 and
 *          with d=2n.
 * https://oeis.org/A088426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of consecutive primes in the arithmetic
 *            progression 19, 19+2n, 19+2*2n, … until the first composite term
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, so n==0 is unused, return 0 */
        return 0;
    const seq_t d = (seq_t)2 * (seq_t)n; /* common difference 2n   */
    seq_t term   = 19;            /* first term of the progression */
    seq_t count  = 0;            /* number of primes seen so far */

    while (true) {
        if (is_prime_big(term))
            count += 1;
        else
            break;
        term += d;               /* move to next term in the progression */
    }
    return count;
}
