/*
 * A132440  Infinitesimal Pascal matrix: generator (lower triangular matrix
 *          representation) of the Pascal matrix, the classical operator xDx,
 *          iterated Laguerre transforms, associated matrices of the list
 *          partition transform and general Euler transformation for
 *          sequences.
 * https://oeis.org/A132440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) = k if n+1 = k(k+3)/2 (k ≥ 1), else 0 (i.e., a(n) is n+1
 *            expressed as a product of consecutive integers k and k+3 divided
 *            by 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* target value: n + 1  (we look for k(k+3)/2 == target) */
    unsigned __int128 target = (unsigned __int128)n + 1;

    if (target < 2U)          /* k=1 gives target=2 */
        return 0;

    unsigned __int128 low  = 1;
    unsigned __int128 high = 1;

    /* find an upper bound for k */
    for (;;)
    {
        unsigned __int128 val = high * (high + 3) / 2;
        if (val >= target)
            break;
        high <<= 1;           /* double the bound */
    }

    /* binary search for k */
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) / 2;
        unsigned __int128 val = mid * (mid + 3) / 2;

        if (val == target)
            return (seq_t)mid;   /* conversion works for all seq_t kinds */

        if (val < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;   /* no integer k satisfies the equation */
}
