/*
 * A213540  Numbers k such that k AND k*2 = 2, where AND is the bitwise AND
 *          operator.
 * https://oeis.org/A213540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003714(n-1)*8 + 3
 * Range:     u8 n <= 13, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003714: numbers whose binary representation has no two consecutive 1's */
/* The sequence a(n) is 8 times such a number plus 3.  */
/* Implementation: iterate over all integers and count those with no consecutive
   1's in binary until the (n‑1)-th is found, then apply the formula.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but safe guard */

    /* need (n-1)-th number in the A003714 sequence */
    idx_t target = n - 1;         /* 0‑based index */
    uint64_t x = 0;               /* candidate integer for A003714 */
    idx_t    seen = 0;            /* count of valid numbers seen so far */

    while (1) {
        /* check for no consecutive 1's : x & (x >> 1) == 0 */
        if ((x & (x >> 1)) == 0) {
            if (seen == target)    /* found the (n-1)-th number */
                break;
            ++seen;
        }
        ++x;
    }

    /* apply the formula: a(n) = 8 * x + 3 */
    uint64_t val64 = (x << 3) + 3ULL;   /* 64‑bit intermediate */
    return (seq_t)val64;                /* truncate to seq_t type */
}
