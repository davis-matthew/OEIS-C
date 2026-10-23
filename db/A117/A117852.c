/*
 * A117852  Mirror image of A098473 formatted as a triangular array.
 * https://oeis.org/A117852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * binomial(2*(r-k), r-k) where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row number, and k = n -
 *            r*(r+1)/2 is the column index in that row
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 137, u64 n <= 470,
 *            u128 n <= 1721, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for 0 <= k <= n.
   Uses a multiplicative method that works for arbitrary-precision
   seq_t and avoids overflow in intermediate steps where possible. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine row r using binary search on triangular numbers. */
    idx_t low = 0, high = n;        /* high always large enough */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= (__int128)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;           /* largest mid with tri <= n */

    /* Compute column k within that row. */
    __int128 start128 = (__int128)r * (r + 1) / 2;
    idx_t row_start = (idx_t)start128;
    idx_t k = n - row_start;

    /* Compute T(r,k) = binomial(r,k) * binomial(2*(r-k), r-k). */
    idx_t m = r - k;                          /* m = n-k */
    seq_t part1 = binom_idx(r, k);
    seq_t part2 = binom_idx(2 * m, m);
    return part1 * part2;
}
