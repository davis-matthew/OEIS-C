/*
 * A161179  A double interspersion, R(n,k), by antidiagonals.
 * https://oeis.org/A161179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by mapping the index n to antidiagonal
 *            coordinates (i, j) and computing R(i, j) using the closed‑form
 *            expressions listed in the OEIS entry
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   compute_R
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_R.h"

seq_t a(idx_t n)
{
    /* Special case for the very first element */
    if (n == 1)
        return (seq_t)1;

    /* Find the diagonal s such that (s-1)*s/2 >= n */
    idx_t low  = 2;
    idx_t high = (n == UINT64_MAX ? UINT64_MAX : n + 1);

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid - 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;   /* diagonal index */

    /* Previous total counts to the end of the previous diagonal */
    unsigned __int128 prev_total128 = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t          prev_total       = (idx_t)prev_total128;

    /* Offset within the current diagonal: 0‑based */
    idx_t offset = n - prev_total - 1;

    /* Coordinates on the diagonal */
    idx_t i = offset + 1;   /* row index */
    idx_t j = s - i;        /* column index */

    return compute_R(i, j);
}
