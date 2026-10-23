/*
 * A158823  Triangle read by rows: matrix product A004736 * A158821.
 * https://oeis.org/A158823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where * r = ceil((sqrt(1+8*n) - 1)/2) – the row
 *            containing index n * c = n - r*(r-1)/2 – the column within the
 *            row * T(r,1) = r*(r+1)/2 * T(r,k) = (r-k+1)*(k-1) , k>1
 * Range:     u8 n <= 253, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of a 128‑bit value, returning floor(sqrt(x)) */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 63;                 /* highest power of two ≤ x */
    while (bit > x) bit >>= 1;              /* find first bit ≤ x */

    unsigned __int128 cur;
    for (; bit != 0; bit >>= 1) {
        cur = (unsigned __int128)(r + bit);
        if (cur * cur <= x)
            r += bit;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* compute the row r that contains index n */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t      s          = isqrt_u128(val);          /* floor(sqrt(1+8*n)) */
    idx_t      r          = (s - 1) / 2;               /* floor version */
    /* adjust upward until cumulative count >= n */
    unsigned __int128 prod = (unsigned __int128)r * (r + 1);
    idx_t      cum  = (idx_t)(prod / 2);
    while (cum < n) {
        ++r;
        prod = (unsigned __int128)r * (r + 1);
        cum  = (idx_t)(prod / 2);
    }

    /* column position within the row */
    unsigned __int128 prodPrev = (unsigned __int128)(r - 1) * r;
    idx_t      prevCum = (idx_t)(prodPrev / 2);
    idx_t      col     = n - prevCum;                 /* 1‑based column */

    /* triangle entry */
    if (col == 1) {
        return ((seq_t)r * (r + 1)) / 2;             /* triangular number */
    } else {
        return ((seq_t)(r - col + 1)) * ((seq_t)(col - 1));
    }
}
