/*
 * A192439  a(n) = a(n-1) - a(n-2) if n is prime or a(n-1) + a(n-2) otherwise.
 *          a(1) = a(2) = 1.
 * https://oeis.org/A192439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; for n>=3, a(n)=a(n-1)-a(n-2) if n is prime,
 *            otherwise a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 23, u16 n <= 39, u32 n <= 74, u64 n <= 137,
 *            u128 n <= 259, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function implements the recurrence directly, using the
    is_prime() helper supplied by the build.  It computes the values
    iteratively, so it works for any order of calls and for any n>=1. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t prevprev = (seq_t)1;   /* a(n-2)  */
    seq_t prev     = (seq_t)1;   /* a(n-1)  */
    seq_t cur;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        if (is_prime(i))
            cur = prev - prevprev;
        else
            cur = prev + prevprev;

        prevprev = prev;
        prev     = cur;
    }

    return prev;
}
