/*
 * A048671  a(n) is the least common multiple of the proper divisors of n.
 * https://oeis.org/A048671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least common multiple of the proper divisors of n
 *            (equivalently: a(n) = n if n is not a prime power; a(n) = n/p if
 *            n = p^k)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 by definition */

    uint64_t nn = (uint64_t)n;
    uint64_t p = 0;                       /* smallest prime factor */

    /* trial division – efficient for n <= 10000 */
    if (nn % 2 == 0) {
        p = 2;
    } else {
        for (uint64_t d = 3; d * d <= nn; d += 2) {
            if (nn % d == 0) { p = d; break; }
        }
    }
    if (p == 0) p = nn;                   /* n itself is prime */

    /* remove all copies of p from nn */
    uint64_t t = nn;
    while (t % p == 0) t /= p;

    /* if after removal nothing remains, nn is a prime power */
    if (t == 1) {
        return (seq_t)nn / (seq_t)p;      /* a(n) = n/p */
    } else {
        return (seq_t)nn;                 /* a(n) = n */
    }
}
