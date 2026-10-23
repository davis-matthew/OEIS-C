/*
 * A196224  Numbers n such that n^2 + n is not of the form x^2 + y^2 + z^2.
 * https://oeis.org/A196224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that n*(n+1) is of the form 4^k*(8*j+7)
 *            (equivalently, n^2+n is not representable as a sum of three
 *            squares)
 * Range:     u8 n <= 22, u16 n <= 5462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many Sirag numbers found so far   */
    idx_t m = 0;          /* current candidate                     */

    seq_t result = 0;     /* value to return                       */

    /* Find the nth Sirag number by brute‑force search.
       For the ranges required by the tests, this runs in well under 2 s. */
    while (count < n)
    {
        ++m;                                 /* next candidate */
        idx_t v = m * (m + 1);                /* v = m*(m+1)    */

        /* Reduce factors of 4: 4^k * (8*j+7) must remain after division. */
        while ((v != 0) && (v % 4 == 0))
            v /= 4;

        if (v % 8 == 7)                     /* forbidden form   */
        {
            ++count;
            result = (seq_t)m;
        }
    }
    return result;
}
