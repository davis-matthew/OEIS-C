/*
 * A177207  Triangle read by rows: R(n,k) = 2^(n-k) mod n.
 * https://oeis.org/A177207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r-k) mod r, where r is the 1‑based row number of the
 *            triangle and k is the column index within that row (row r
 *            contains the terms R(r,1)…R(r,r) = 2^(r-1)…2^0 (mod r))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for n < 1 */

    /* Find the row r such that offset_row(r) ≤ n < offset_row(r+1).
       offset_row(r) = (r-1)*r/2 + 1.  Use binary search. */
    idx_t low = 1;
    idx_t high = n;                     /* r ≤ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tr = (__int128)mid * (mid + 1) / 2;
        if (tr >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the offset of the first element of row r. */
    unsigned __int128 offset128 = (__int128)(r - 1) * r / 2 + 1;
    idx_t offset = (idx_t)offset128;

    /* Column index k (1-based). */
    idx_t k = n - offset + 1;

    /* Exponent e = r - k. */
    idx_t e = r - k;

    /* Modulus = r.  Compute 2^e mod r using the provided utility
       which works for all seq_t sizes (including arbitrary-precision). */
    seq_t mod_val = (seq_t)r;
    seq_t result = mod_pow((seq_t)2, e, mod_val);

    return result;
}
