/*
 * A130328  Triangle of differences between powers of 2, read by rows.
 * https://oeis.org/A130328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r+1) - 2^k * where r = floor((sqrt(8*(n+1)+1)-3)/2) is
 *            the row number, * k = n - r*(r+1)/2 is the column number. The
 *            sequence is * obtained by reading the triangle
 *            t(n,k)=2^(n+1)-2^k for * n>=0, k=0..n in row‑major order
 * Range:     u8 n <= 35, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find smallest r such that (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while (1) {
        /* use 128‑bit to avoid overflow for large n */
        unsigned __int128 tri = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (tri > n) {
            break;
        }
        ++r;
    }

    /* index of the first element of row r */
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prev;   /* column index, 0 <= k <= r */

    /* compute 2^(r+1) and 2^k using the provided pow2 utility, which
     * is defined for all supported seq_t types without shifting beyond
     * the type width. */
    seq_t high = pow2(r + 1);
    seq_t low  = pow2(k);

    return high - low;
}
