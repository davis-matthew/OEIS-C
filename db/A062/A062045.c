/*
 * A062045  Positive numbers whose product of digits is 12 times their sum.
 * https://oeis.org/A062045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers n such that product_of_digits(n) = 12 × sum_of_digits(n)
 *            (computed by exhaustive search)
 * Range:     u8 none, u16 n <= 307, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a given number satisfies the condition. */
static bool satisfies(seq_t x)
{
    seq_t sum = 0;
    seq_t prod = 1;
    seq_t t   = x;

    /* Work over the decimal digits of x. */
    while (t > 0)
    {
        seq_t d = t % 10;   /* last decimal digit */
        sum += d;
        prod *= d;
        t /= 10;
    }

    return prod == sum * 12;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t  found     = 0;
    seq_t  candidate = 1;

    while (found < n)
    {
        if (satisfies(candidate))
        {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }

    /* This point is never reached for valid n. */
    return 0;
}
