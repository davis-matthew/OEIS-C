/*
 * A218831  a(n) is the least r > 1 for which the interval (r*n, r*(n+1))
 *          contains no prime, or a(n)=0 if no such r exists.
 * https://oeis.org/A218831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer r>1 such that the open interval *
 *            (r*n, r*(n+1)) contains no prime; if no such r exists a(n)=0. *
 *            * The algorithm searches r from 2 upward, testing whether the
 *            interval * contains a prime by finding the first prime > r*n and
 *            checking if it * lies before r*(n+1). It stops...
 * Range:     u8 none, u16 n <= 9575, u32 n <= 9575, u64 n <= 9575,
 *            u128 n <= 9575, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Upper limit for r to search.  The known maximal value in the
       sequence up to 5·10^7 is 16, so 1000 is comfortably above that
       and keeps the search short while still covering all verified
       terms. */
    const idx_t R_MAX = 1000;

    for (idx_t r = 2; r <= R_MAX; ++r)
    {
        /* Compute the bounds of the interval (r*n, r*(n+1)) as seq_t
           to avoid overflow; all intermediate values fit in seq_t
           because r and n are small for the search range. */
        seq_t low  = (seq_t)r * (seq_t)n + 1;     /* first possible prime > r*n      */
        seq_t high = (seq_t)r * (seq_t)(n + 1);   /* upper bound of the interval      */

        /* Find the first prime greater than r*n. */
        seq_t p = low;
        while (!is_prime_big(p))
            ++p;

        /* If that prime is not inside the interval, r is the answer. */
        if (p >= high)
            return (seq_t)r;
    }

    /* If we reach here no r up to R_MAX satisfies the condition;
       by definition a(n) = 0. */
    return (seq_t)0;
}
