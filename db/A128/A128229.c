/*
 * A128229  A natural number transform, inverse of signed A094587.
 * https://oeis.org/A128229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is a triangular number T_k = k(k+1)/2, * = k if n
 *            = T_k + k, * otherwise 0. * Indexed with offset 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = 1, high = n, best = 0;

    /* Binary search for the largest k such that T_k <= n */
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
        {
            best = mid;
            low  = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    unsigned __int128 Tk = (__int128)best * (best + 1) / 2;

    if (Tk == n)
        return 1;                       /* n is a triangular number */
    if ((__int128)(n - Tk) == best)
        return (seq_t)best;              /* n = T_k + k */
    return 0;                            /* all other cases */
}
