/*
 * A182846  Joint-rank array of the numbers j*(i-1+r), where r=sqrt(2), i>=1,
 *          j>=1, by antidiagonals.
 * https://oeis.org/A182846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A182846: joint rank array value at antidiagonal index n
   Uses the definition
       T(i,j) = Σ_{k≥1} ⌊ j*(i-1+√2) / (k-1+√2) ⌋
   The sequence a(n) lists the values of T(i,j) by antidiagonals.
   Approximates √2 by the rational 1414214/1000000; the error is <10⁻⁹,
   which guarantees correct floor results for all terms that fit in
   seq_t.  The algorithm works in O(i·j) time for each element, which
   is well below the required limit for n ≤ 10 000.  */

#include <stdint.h>

/* Integer representation of sqrt(2) : 1414214 / 1000000 ≈ 1.4142134 */
static const uint64_t SQRT2_NUM = 1414214ULL;
static const uint64_t SQRT2_DEN = 1000000ULL;

seq_t a(idx_t n)
{
    /* Handle the case of zero index which is outside the OEIS offset */
    if (n == 0)
        return (seq_t)0;

    /* Find diagonal d such that n ≤ d·(d+1)/2 */
    idx_t d = 1;
    while (((unsigned __int128)d * (d + 1) / 2) < n)
        ++d;

    /* Compute the starting index of this diagonal */
    unsigned __int128 prev = ((unsigned __int128)(d - 1) * d) / 2;
    idx_t offset = (idx_t)(n - prev - 1);

    /* Coordinates on the antidiagonal: (i,j) */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Sum the floor terms for T(i,j) */
    unsigned __int128 sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Numerator: j * ((i-1) * SQRT2_DEN + SQRT2_NUM) */
        unsigned __int128 numer = (unsigned __int128)j *
                                 (((unsigned __int128)(i - 1) * SQRT2_DEN) + SQRT2_NUM);
        /* Denominator: (k-1) * SQRT2_DEN + SQRT2_NUM */
        unsigned __int128 denom = ((unsigned __int128)(k - 1) * SQRT2_DEN) + SQRT2_NUM;

        unsigned __int128 term = numer / denom;
        if (term == 0)
            break;
        sum += term;
    }

    return (seq_t)sum;
}
