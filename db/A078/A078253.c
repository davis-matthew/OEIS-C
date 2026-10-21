/*
 * A078253  a(n) = A078252(n)/n.
 * https://oeis.org/A078253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) equals the smallest positive integer m such that the
 *            product n · m is an n‑digit decimal number whose digits
 *            alternate between two decimal digits (the two digits may be
 *            equal, in which case the number consists of a single repeated
 *            digit). If no such m exists, a(n) is 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* search over all pairs of digits (a,b).  a must be 1..9 (no leading
       zero) and b may be 0..9.  The pairs are examined in lexicographic
       order (a ascending, then b ascending).  The first pair for which
       the n‑digit alternating number is divisible by n is the minimal
       candidate. */
    for (uint8_t a = 1; a <= 9; ++a)
    {
        for (uint8_t b = 0; b <= 9; ++b)
        {
            /* compute remainder of the n‑digit alternating number modulo n. */
            idx_t rem = 0;
            for (idx_t i = 0; i < n; ++i)
            {
                uint8_t digit = (i & 1U) ? b : a;   /* i even -> a, odd -> b */
                rem = (rem * 10 + digit) % n;
            }

            if (rem != 0)
                continue;   /* this pair does not produce a multiple of n */

            /* the pair (a,b) yields a valid number.  Compute the quotient
               (m = (n·m)/n) by performing integer division of the
               alternating number by n. */
            seq_t q = 0;          /* the resulting m */
            rem = 0;              /* reset remainder for division */
            for (idx_t i = 0; i < n; ++i)
            {
                uint8_t digit = (i & 1U) ? b : a;
                idx_t tmp = rem * 10 + digit;
                seq_t qdigit = (seq_t)(tmp / n);
                rem = tmp % n;
                q = q * 10 + qdigit;
            }
            /* due to the remainder check above, rem must be 0 here. */
            return q;
        }
    }
    /* no alternating‑digit n‑digit multiple exists */
    return 0;
}
