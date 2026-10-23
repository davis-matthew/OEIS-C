/*
 * A102096  Second greatest (or second least) edge length of a cuboid having
 *          integer edge lengths, volume n and minimal surface area under
 *          those restrictions.
 * https://oeis.org/A102096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second largest edge length of the cuboid with integer
 *            edges and volume n that has minimal surface area; equivalent to
 *            minimizing the sum a+b+c of the edge lengths
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, but guard */

    idx_t a_lim = 0;
    seq_t nSeq = (seq_t)n;            /* cast to seq_t for arithmetic */

    /* Upper bound for the first edge: cube root of n */
    for (a_lim = 1; (seq_t)a_lim * a_lim * a_lim <= nSeq; ++a_lim) { }

    bool first = true;
    seq_t bestSum = 0, bestArea = 0, bestB = 0;

    for (idx_t a = 1; a < a_lim; ++a)
    {
        if (n % a != 0)
            continue;                 /* a must divide n */

        idx_t rem1 = n / a;           /* remaining product for b·c */

        /* upper bound for the second edge: sqrt(rem1) */
        idx_t b_lim = 0;
        for (b_lim = 1; (seq_t)b_lim * b_lim <= (seq_t)rem1; ++b_lim) { }

        for (idx_t b = a; b < b_lim; ++b)
        {
            if (rem1 % b != 0)
                continue;             /* b must divide rem1 */

            idx_t c = rem1 / b;       /* third edge */

            /* compute sum and surface area using seq_t arithmetic */
            seq_t sum   = (seq_t)a + (seq_t)b + (seq_t)c;
            seq_t area  = (seq_t)2 *
                           ((seq_t)a * b + (seq_t)a * c + (seq_t)b * c);

            if (first || sum < bestSum ||
                (sum == bestSum && area < bestArea))
            {
                first   = false;
                bestSum = sum;
                bestArea = area;
                bestB   = (seq_t)b;   /* b is the second largest */
            }
        }
    }

    return bestB;
}
