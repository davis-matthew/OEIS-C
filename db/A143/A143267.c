/*
 * A143267  Triangle read by rows, A130207 * A000012 * A127648.
 * https://oeis.org/A143267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = φ(r) * k where r is the unique integer with r*(r+1)/2 ≥ n
 *            and k = n – r*(r‑1)/2. The sequence is the rows of the triangle
 *            T(n,k) = φ(n)·k read by rows
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _phi_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_phi_idx.h"

seq_t a(idx_t N)
{
    if (N == 0)   /* below the offset; undefined but return 0 */
        return 0;

    /* Find the row r such that r*(r+1)/2 ≥ N and (r‑1)*r/2 < N. */
    idx_t low = 1, high = N;                           /* high is a safe upper bound */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(N - prev_tri);                   /* position within row r */

    seq_t phi_r = _phi_idx(r);
    return phi_r * (seq_t)k;
}
