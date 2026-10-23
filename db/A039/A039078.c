/*
 * A039078  Numbers whose base-9 representation has the same number of 0's and
 *          2's.
 * https://oeis.org/A039078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th positive integer whose base‑9 representation has
   the same number of 0's and 2's by a simple linear search. */

#include <stdbool.h>

/* Count zeroes and twos in the base‑9 expansion of x. */
static bool equal_zero_two(seq_t x)
{
    unsigned int c0 = 0, c2 = 0;
    seq_t t = x;
    while (t != (seq_t)0)
    {
        unsigned int d = (unsigned int)(t % (seq_t)9);
        if (d == 0) ++c0;
        else if (d == 2) ++c2;
        t /= (seq_t)9;
    }
    return c0 == c2;
}

/* Return the nth element of sequence A039078 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* guard for out of bounds */

    seq_t candidate = (seq_t)1;   /* start testing from 1 */
    idx_t seen = 0;

    while (seen < n)
    {
        if (equal_zero_two(candidate))
        {
            ++seen;
            if (seen == n)
                return candidate;
        }
        ++candidate;
    }

    return candidate;   /* unreachable, kept for completeness */
}
