/*
 * A129654  Number of different ways to represent n as general polygonal
 *          number P(m,r) = 1/2*r*((m-2)*r-(m-4)) = n>1, for m,r>1.
 * https://oeis.org/A129654
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(m,r) | m>1, r>1, P(m,r) = n}, where P(m,r) =
 *            r*((m-2)*r-(m-4))/2 . Implementation enumerates all r>1 that
 *            divide 2*n, checks whether m computed from the equation is an
 *            integer >1, and counts the number of such pairs
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are undefined */
        return 0;

    seq_t count = 0;
    seq_t two_n = (seq_t)n * 2;      /* 2*n, safe in unsigned seq_t */

    for (idx_t r = 2; r <= n; ++r)   /* r cannot exceed n (P(2,r)=r) */
    {
        if (two_n % r != 0)
            continue;                /* r must divide 2*n */

        seq_t term = two_n / r + (seq_t)2 * r - 4;   /* numerator for m */
        if (term % (r - 1) != 0)
            continue;                /* m would not be integer */

        seq_t m = term / (r - 1);
        if (m > 1)
            ++count;
    }

    return count;
}
