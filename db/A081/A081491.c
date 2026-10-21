/*
 * A081491  Triangle read by rows in which the n-th row contains n terms of an
 *          arithmetic progression with a common difference of (n-1) and the
 *          first term of (n+1)-th row is 1 more than the last term of the
 *          n-th row.
 * https://oeis.org/A081491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(r) + (c-1)(r-1) * Where * r = ceil( (-1 + sqrt(1 + 8n))
 *            / 2 ) (row index, 1‑based) * c = n - r(r-1)/2 (column index in
 *            that row) * S(r) = r + (r-2)(r-1)(2r-3)/6 (first term of row r)
 *            * * All arithmetic is performed with 128‑bit integers where
 *            needed to avoid overflow
 * Range:     u8 n <= 50, u16 n <= 1748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for __uint128_t (returns floor(sqrt(x))) */
static uint64_t isqrt128(__uint128_t x)
{
    uint64_t r = 0;
    for (int k = 127; k >= 0; --k) {
        uint64_t cand = r | (1ULL << k);
        if ((__uint128_t)cand * cand <= x)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset is 1, behaviour for n=0 is irrelevant */

    /* Find row r: smallest r with r(r+1)/2 >= n */
    __uint128_t twon = (__uint128_t)8 * n + 1;         /* 1 + 8n   */
    uint64_t sqrt_val = isqrt128(twon);                /* floor sqrt(1+8n) */
    idx_t r = (idx_t)((sqrt_val - 1) / 2);

    /* Ensure r is the correct row (ceil) */
    while ((__uint128_t)r * (r + 1) / 2 < n)
        ++r;

    /* Column within the row */
    idx_t prev_tri = (idx_t)((__uint128_t)r * (r - 1) / 2); /* C(r-1) */
    idx_t c = n - prev_tri;                               /* 1‑based */

    /* First term of row r: S(r) = r + (r-2)(r-1)(2r-3)/6 */
    __int128 prod = (__int128)(r - 2) * (r - 1) * (2 * r - 3);
    seq_t S = (seq_t)r + (seq_t)(prod / 6);

    /* Final term: S + (c-1)(r-1) */
    seq_t term = S + (seq_t)(c - 1) * (seq_t)(r - 1);
    return term;
}
