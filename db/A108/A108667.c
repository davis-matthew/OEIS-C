/*
 * A108667  Triangle read by rows: T(n,k) = 9k*n + 14(n+k) + 20 (0 <= k <= n).
 * https://oeis.org/A108667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 9*r*c + 14*(r + c) + 20, where * r = floor((sqrt(1 + 8*n)
 *            – 1) / 2) * c = n – r*(r + 1)/2 (row‐major order of the
 *            triangle)
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns the greatest x such that x*x <= v. */
static inline uint64_t isqrt128(unsigned __int128 v)
{
    uint64_t low = 0, high = (uint64_t)~0ULL;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Compute a(n) for the OEIS sequence A108667. */
seq_t a(idx_t n)
{
    /* Compute the row index r using the inverse triangular number formula. */
    unsigned __int128 d = (unsigned __int128)1 + (unsigned __int128)8 * n;
    uint64_t s = isqrt128(d);                   /* floor(sqrt(1 + 8*n)) */
    idx_t r = (idx_t)((s - 1) / 2);              /* floor((sqrt(1+8n)-1)/2) */

    /* Number of terms before row r: r*(r+1)/2. */
    idx_t cum_prev = (idx_t)((seq_t)r * (r + 1) / 2);

    /* Column index within the current row. */
    idx_t c = n - cum_prev;

    /* Triangle value: 9*k*n + 14*(n+k) + 20 with k = c, n = r. */
    seq_t val = (seq_t)9 * (seq_t)r * (seq_t)c
              + (seq_t)14 * ((seq_t)r + (seq_t)c) + (seq_t)20;
    return val;
}
