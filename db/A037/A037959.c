/*
 * A037959  a(n) = n^2*(n+1)*(n+2)!/48.
 * https://oeis.org/A037959
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n+1)*(n+2)!/48 (offset 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2 (offset 2). For n < 2 return 0. */
    if (n < 2)
        return 0;

    seq_t rem = 48;   /* remaining divisor to cancel   */
    seq_t res = 1;    /* accumulated product (after cancellations) */

    /* Process the factorial part (2 .. n+2). */
    for (idx_t i = 2; i <= n + 2; ++i)
    {
        seq_t f = (seq_t)i;
        seq_t g = gcd(f, rem);
        if (g > 1)
        {
            rem /= g;
            f   /= g;
        }
        res *= f;
    }

    /* Process the remaining multiplicative factors: n, n, and n+1. */
    seq_t factors[3] = { (seq_t)n, (seq_t)n, (seq_t)(n + 1) };
    for (int k = 0; k < 3; ++k)
    {
        seq_t f = factors[k];
        seq_t g = gcd(f, rem);
        if (g > 1)
        {
            rem /= g;
            f   /= g;
        }
        res *= f;
    }

    /* Any remaining part of the divisor (rem) must now be 1. */
    if (rem > 1)
        res *= rem;   /* safety, though rem should be 1 here */

    return res;
}
