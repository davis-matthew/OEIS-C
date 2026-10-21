/*
 * A223346  3 X 3 X 3 triangular graph without horizontal edges coloring a
 *          rectangular array: number of n X 1 0..5 arrays where 0..5 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 and every
 *          array movement to a horizontal or vertical neighbor moves along an
 *          edge of this graph.
 * https://oeis.org/A223346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 6, a(2) = 12, a(n) = 12 * 5^((n/2)-1) for even n ≥ 2,
 *            a(n) = 28 * 5^((n-3)/2) for odd n ≥ 3 (for n = 0 the definition
 *            gives 6, though this value is out of range)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation of 5 with an unsigned integer exponent. */
static seq_t pow5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset is 1; n = 0 is not used, return 6 to match the OEIS extension. */
    if (n == 0)
        return 6;

    if (n == 1)
        return 6;
    if (n == 2)
        return 12;

    if ((n & 1) == 0)          /* even n */
    {
        idx_t e = (n / 2) - 1;
        return ((seq_t)12) * pow5(e);
    }
    else                        /* odd n ≥ 3 */
    {
        idx_t e = (n - 3) / 2;
        return ((seq_t)28) * pow5(e);
    }
}
