/*
 * A202071  Number of arrays of n+2 integers in -3..3 with sum zero and the
 *          sum of every adjacent pair being odd.
 * https://oeis.org/A202071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   count_pattern, dp_even, dp_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202071
 * Number of arrays of length n+2 of integers in {-3,-2,-1,0,1,2,3}
 * whose total sum is zero and such that the sum of each adjacent pair
 * is odd.  This property forces the array to alternate in parity,
 * so the array consists of "odd" elements (±1,±3) on the odd
 * positions and "even" elements (−2,0,2) on the even positions.
 * The following routine computes the number of such arrays for any
 * n ≥ 1 by dynamic programming for the two parity classes.  The
 * algorithm is O(n²) in the worst case, fast enough for the
 * required limits (n ≈ 10 000).  It uses only the integer operations
 * permitted in the problem statement and works for any seq_t
 * (unsigned integer of arbitrary width).
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_pattern.h"
#include "../utils/dp_even.h"
#include "../utils/dp_odd.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Length of the array is n+2 */
    idx_t L = n + 2;
    /* Number of odd‑positioned elements for the two possible patterns */
    idx_t m_odd_start  = (L + 1) / 2;   /* ceil(L/2)   */
    idx_t m_even_start = L / 2;         /* floor(L/2)  */

    seq_t res = 0;
    res += count_pattern(m_odd_start,  m_even_start); /* odd first */
    res += count_pattern(m_even_start, m_odd_start);  /* even first */
    return res;
}
