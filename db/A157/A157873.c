/*
 * A157873  Triangle T(n,m)= binomial(n,2^m) + binomial(n,2^(n - m)) read by
 *          rows.
 * https://oeis.org/A157873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A157873: the sequence obtained by reading the triangle
   T(n,m)=binomial(n,2^m)+binomial(n,2^(n-m)) row by row.  The index
   n refers to the position in this flattened array, with n=0 being
   the first entry (0). */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = ((idx_t)1 << 32);  /* enough for all 64‑bit n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    /* Offset within the row: m */
    idx_t row_start = (__int128)r * (r + 1) / 2;
    idx_t m = n - row_start;
    /* Compute T(r,m) = binomial(r,2^m)+binomial(r,2^(r-m)) */
    seq_t val = 0;

    /* First term: binomial(r, 2^m) */
    if (m < 64) {
        idx_t k1 = ((idx_t)1 << m);
        if (k1 <= r)
            val += binomial(r, k1);
    }

    /* Second term: binomial(r, 2^(r-m)) */
    idx_t shift = r - m;
    if (shift < 64) {
        idx_t k2 = ((idx_t)1 << shift);
        if (k2 <= r)
            val += binomial(r, k2);
    }

    return val;
}
