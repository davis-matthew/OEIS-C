/*
 * A140531  Concatenate subsequences 0, 1, 2, 4, ..., 2^k.
 * https://oeis.org/A140531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(2^(n-2-t*(t+1)/2)), where
 *            t=floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 53, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A140531 (concatenation of 0,1,2,4,...).  */
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

/* Integer square‑root of a 128‑bit unsigned integer, floor. */
static inline idx_t isqrt_u128(unsigned __int128 val)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32;          /* high is > sqrt(max(8*n+1)) for 0<=n<2^64 */
    if (high > val) high = val;           /* adjust for very small values */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 midsq = (unsigned __int128)mid * mid;
        if (midsq <= val)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function: a(n) for n>=0, with external linkage */
seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 0)
        return 0;

    /* Compute r = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 m = (unsigned __int128)8 * n + 1;   /* 8*n + 1, fits in 128 bits */
    idx_t      sqrt_val = isqrt_u128(m);                 /* floor(sqrt(m)) */
    idx_t      r        = (sqrt_val - 1) / 2;             /* row index */

    /* offset = r*(r+1)/2, computed safely in 128 bits */
    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) >> 1);

    /* Position within the row */
    idx_t pos = n - offset;

    if (pos == 0)      /* first element of each row is 0 */
        return 0;

    /* Subsequent elements are powers of two: 2^(pos-1) */
    return pow2(pos - 1);
}
