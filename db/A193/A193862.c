/*
 * A193862  Mirror of the triangle A115068.
 * https://oeis.org/A193862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the row index such that r*(r+1)/2 <= n < (r+1)*(r+2)/2,
 *            and let k = n - r*(r+1)/2 (the column index). Then a(n) = 2^k *
 *            binom(r+1, k+1). Here binom(a,b) is the binomial coefficient
 *            C(a,b)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute the n-th term of OEIS A193862 */
seq_t a(idx_t n)
{
    /* Find the row r with binary search using 128‑bit arithmetic
       to avoid overflow.  r satisfies r*(r+1)/2 <= n. */
    idx_t low = 0;
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high == 0) break;          /* safety against wrap‑around */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute prefix count of elements before row r */
    idx_t prefix = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - prefix;              /* column index within the row */

    /* w(r,k) = 2^k * binom(r+1, k+1) */
    seq_t bin = binom(r + 1, k + 1);
    seq_t pow2k = ((seq_t)1) << k;    /* 2^k  (works for 8/16/32/64/128 bits and most bigint types) */

    return bin * pow2k;
}
