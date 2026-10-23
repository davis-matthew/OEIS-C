/*
 * A044392  Numbers n such that string 6,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that contains the decimal
 *            substring "60", but n-1 does not. For example, a(1) = 60, a(2) =
 *            160, a(3) = 260, …
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff the decimal representation of x contains the
   substring "60" (i.e., a digit 6 followed immediately by a digit 0). */
static bool has60(seq_t x)
{
    while (x >= 10)
    {
        if (x % 100 == 60)      /* check the last two digits */
            return true;
        x /= 10;                /* shift right by one decimal digit */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n = 0, just return 0 */

    /* First term is always 60. */
    if (n == 1)
        return (seq_t)60;

    /* Search for the n‑th term. */
    seq_t curr = (seq_t)60;
    idx_t  count = 1;          /* we already have the first term */

    while (count < n)
    {
        ++curr;                 /* move to the next integer */
        /* n-1 must not contain "60", n must contain "60". */
        if (!has60(curr - 1) && has60(curr))
            ++count;
    }
    return curr;
}
