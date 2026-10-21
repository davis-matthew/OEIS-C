/*
 * A092582  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] having length of first run equal to k.
 * https://oeis.org/A092582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r is the smallest integer with r*(r+1)/2 >=
 *            n, and c = n - r*(r-1)/2 (1 ≤ c ≤ r). T(n,k) = 1 if k == n;
 *            otherwise T(n,k) = k * Π_{i=k+2}^{n} i (equivalent to
 *            k*n!/(k+1)! for k < n)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 91, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find r: smallest s.t. r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* previous total elements before row r */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - prev;             /* column, 1‑based */

    if (r == c)                     /* T(n,n) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)c;        /* start with factor k */
    for (idx_t i = c + 2; i <= r; ++i)
        result *= (seq_t)i;         /* multiply remaining terms */

    return result;
}
