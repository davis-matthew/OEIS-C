/*
 * A224713  The number of unordered partitions {a, b} of the even numbers 2n
 *          such that a or b is composite and the other is prime.
 * https://oeis.org/A224713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of unordered partitions {a,b} of 2n such that one part
   is prime and the other is composite.  The result is returned as a seq_t. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* not used, but guard against 0 index */
        return 0;

    idx_t even = 2 * n;          /* the even number 2n */
    idx_t limit = even / 2;      /* a runs from 1 to n (unordered pairs) */
    seq_t count = 0;

    for (idx_t a = 1; a <= limit; ++a)
    {
        idx_t b = even - a;

        /* a == b gives equal parts – they cannot satisfy “prime & composite” */
        if (a == b)
            continue;

        bool aPrime = is_prime(a);
        bool bPrime = is_prime(b);

        /* composite means >1 and not prime */
        bool aComposite = (a > 1) && !aPrime;
        bool bComposite = (b > 1) && !bPrime;

        if ((aPrime && bComposite) || (bPrime && aComposite))
            ++count;
    }

    return count;
}
