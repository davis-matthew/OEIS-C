/*
 * A101508  Product of binomial matrix and the Mobius matrix A051731.
 * https://oeis.org/A101508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((√(8*n + 1) - 1)/2), c = n -
 *            r*(r+1)/2. T(r,c) = Σ_{m≥1, m*(c+1) ≤ r+1} C(r, m*(c+1)-1)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8254, big any n
 * Validated: standard, pass, 94 terms through n = 9098 (2026-11-03)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* Compute T(row, col) as defined above.  */
static inline seq_t compute_T(idx_t row, idx_t col)
{
    seq_t sum = 0;
    idx_t step = col + 1;

    /* i = 0 case */
    if (step == 1)
        sum += 1;          /* C(row,0) = 1 */

    seq_t bin = 1;          /* C(row,0) */
    for (idx_t i = 1; i <= row; ++i) {
        /* update bin = C(row,i) */
        bin = bin * (seq_t)(row - i + 1) / (seq_t)i;
        if ( (i + 1) % step == 0 )   /* (i+1) divisible by (col+1) */
            sum += bin;
    }
    return sum;
}

/* Main sequence function required by the build system.  */
seq_t a_wide(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0, hi = n;      /* upper bound hi cannot exceed n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t row = lo;
    idx_t start = row * (row + 1) / 2;
    idx_t col = n - start;

    return compute_T(row, col);
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
