/*
 * A119828  Number triangle T(n,k)=(2n)!/(2k)!.
 * https://oeis.org/A119828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) = (2r)!/(2k)!, where r and k are the row and
 *            column of n in the triangle T defined by T(m,t) = (2m)!/(2t)!.
 *            The sequence lists the triangle row‑by‑row: indices 0..n are
 *            filled by rows m=0,1,2,... so a(n) = T(r,k) with r chosen so
 *            that r(r+1)/2 <= n < (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for the OEIS sequence A119828 */
seq_t a(idx_t n)
{
    /* Find r so that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = n;                /* upper bound – r never exceeds n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1U) / 2U;
        if (val <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Compute k = n - r(r+1)/2 */
    unsigned __int128 prefix_128 = (unsigned __int128)r * (r + 1U) / 2U;
    idx_t prefix = (idx_t)prefix_128;
    idx_t k = n - prefix;

    /* Result = product_{j=2k+1}^{2r} j  (with product of empty set = 1) */
    seq_t result = 1;
    for (idx_t j = 2*k + 1; j <= 2*r; ++j)
        result *= (seq_t)j;

    return result;
}
