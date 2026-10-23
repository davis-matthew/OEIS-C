/*
 * A162613  Triangle read by rows in which row n lists n terms, starting with
 *          n, with gaps = n^2-1 between successive terms.
 * https://oeis.org/A162613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r + (k-1)*(r^2-1), where r is the unique integer
 *            satisfying r(r-1)/2 < n <= r(r+1)/2, and k = n - r(r-1)/2
 * Range:     u8 n <= 27, u16 n <= 859, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit value, returning floor(sqrt(x)). */
static inline idx_t
isqrt_u128(unsigned __int128 x)
{
    /* sqrt(8*n+1) is < 2^34 for 0 <= n < 2^64, so a 35‑bit search range is safe. */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 35;          /* 2^35 > sqrt(8*2^64+1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                          /* floor sqrt(x) */
}

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined; return 0 (won't be called). */
    if (n == 0)
        return 0;

    /* Find the row r that contains index n. */
    unsigned __int128 target = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(target);           /* floor sqrt(8*n+1) */
    idx_t r = (s - 1) / 2;                   /* initial estimate */

    /* Correct possible off‑by‑one errors. */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    if (tri_r < n)
        ++r;                                 /* most often needed */

    /* Position k within the r‑th row (1‑based). */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_tri);

    /* Compute the term. */
    seq_t rr  = (seq_t)r;
    seq_t gap = rr * rr - 1;                 /* (r^2 - 1) */
    seq_t val = rr + (seq_t)(k - 1) * gap;   /* r + (k-1)*(gap) */

    return val;
}
