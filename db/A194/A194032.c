/*
 * A194032  Natural interspersion of the squares (1,4,9,16,25,...), a
 *          rectangular array, by antidiagonals.
 * https://oeis.org/A194032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (col + max(floor(row/2)-1,0))^2 + row - 1 * where
 *            (row,col) is obtained by traversing the array * T(row,col) =
 *            (col + max(floor(row/2)-1,0))^2 + row - 1 * along antidiagonals
 *            (rows increase inside each diagonal)
 * Range:     u8 n <= 120, u16 n <= 9064, u32 n <= 9064, u64 n <= 9064,
 *            u128 n <= 9064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compile with a compiler that supports __uint128_t (e.g. GCC/Clang). */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)).  Standard bit‑by‑bit algorithm, 64 iterations. */
static uint64_t isqrt_uint128(__uint128_t x)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i)
    {
        uint64_t bit = 1ULL << i;
        __uint128_t test = (__uint128_t)(r + bit) * (r + bit);
        if (test <= x)
            r |= bit;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is 1‑based; return 0 for undefined n=0 */
        return 0;

    /* 1. Find diagonal index d (d >= 1) such that
     *    sum_{i=1}^{d} i = d(d+1)/2  >= n.
     *  Compute d via the quadratic formula with integer square root.
     */
    __uint128_t m1 = (__uint128_t)8 * n + 1;           /* 8*n + 1 fits in 128 bits */
    uint64_t t = isqrt_uint128(m1);                    /* floor(sqrt(8*n+1))     */
    uint64_t d = (t - 1) / 2;                          /* floor((sqrt(8n+1)-1)/2) */

    /* Adjust to the ceil if needed */
    if ( (__uint128_t)d * (d + 1) / 2 < n )
        ++d;                                           /* now d satisfies d(d+1)/2 >= n */

    /* 2. Total elements before this diagonal */
    __uint128_t prev = (__uint128_t)(d - 1) * d / 2;    /* sum_{i=1}^{d-1} i */

    /* 3. Offset within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;                       /* offset ∈ [0, d-1]       */

    /* 4. Convert to (row,col) coordinates.
     *    Rows increase inside a diagonal; row = offset+1.
     *    Sum of coordinates = d+1; thus col = (d+1) - row.
     */
    idx_t row = offset + 1;
    idx_t col = (d + 1) - row;

    /* 5. Compute the entry using the formula from the OEIS entry. */
    uint64_t shift = row >> 1;                          /* floor(row/2)           */
    uint64_t add   = (shift > 0) ? shift - 1 : 0;       /* max(floor(row/2)-1,0)  */
    uint64_t base  = col + add;                         /* inner value for square */

    seq_t base_sq = (seq_t)base * (seq_t)base;
    seq_t result  = base_sq + (seq_t)row - 1;

    return result;
}
