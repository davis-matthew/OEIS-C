/*
 * A085558  Numbers that have more nonprime digits than prime digits.
 * https://oeis.org/A085558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th nonnegative integer that has more nonprime
 *            digits * (0,1,4,6,8,9) than prime digits (2,3,5,7) when written
 *            in base 10. * Sequence A085558
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>          /* for NULL, malloc, free if needed */

/* Helper: tell whether a decimal digit is prime. */
static const bool prime_digit[10] =
{
    false,           /* 0 */
    false,           /* 1 */
    true,            /* 2 */
    true,            /* 3 */
    false,           /* 4 */
    true,            /* 5 */
    false,           /* 6 */
    true,            /* 7 */
    false,           /* 8 */
    false            /* 9 */
};

/* Does x have more nonprime digits than prime digits? */
static inline bool more_nonprime_digits(seq_t x)
{
    seq_t v = x;
    int diff = 0;     /* +1 for nonprime, -1 for prime */

    if (v == 0)       /* the number zero is a single digit '0' (nonprime) */
    {
        diff = 1;
    }
    else
    {
        while (v > 0)
        {
            int d = (int)(v % 10);    /* digit */
            diff += prime_digit[d] ? -1 : 1;
            v /= 10;
        }
    }
    return diff > 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Edge case: n < 1 is undefined; just return 0. */
    if (n < 1)
        return (seq_t)0;

    seq_t current = 0;          /* candidate number */
    idx_t found  = 0;           /* number of qualifying terms found */

    while (found < n)
    {
        if (more_nonprime_digits(current))
        {
            ++found;
        }
        ++current;
    }

    /* current is one past the desired term */
    --current;
    return current;
}
