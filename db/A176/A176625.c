/*
 * A176625  T(n,k) = 1 + 3*k*(k - n), triangle read by rows (n >= 0, 0 <= k <=
 *          n).
 * https://oeis.org/A176625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the linearization of the triangle T(n,k) = 1 + 3*k*(k - n)
 *          read by rows: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *
 * The computation uses integer arithmetic only, with careful casting to
 * avoid unsigned underflow when k < r.
 */

#include <stdint.h>

/* Return floor(sqrt(v)) for a non‑negative __int128 value v.
 *  v is guaranteed to fit in __int128 (≥0). */
static inline idx_t floor_sqrt_uint128(__int128 v)
{
    idx_t lo = 0;
    /* 2^34 ≈ 1.7e10, which is > sqrt(8*2^64+1) */
    idx_t hi = ((idx_t)1 << 34);
    idx_t ans = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= v)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Return the nth term of OEIS A176625. */
seq_t a(idx_t n)
{
    /* Find the row `r` such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    __int128 tmp = (__int128)n * 8 + 1;          /* (8*n+1) in 128‑bit */
    idx_t sqrt_val = floor_sqrt_uint128(tmp);
    idx_t r = (sqrt_val - 1) / 2;

    /* Compute the linear index offset of the start of row `r`. */
    __int128 off128 = (__int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;

    /* Column index within the row. */
    idx_t k = n - offset;

    /* Compute T(r,k) = 1 + 3*k*(k - r) using signed arithmetic. */
    seq_t k_seq = (seq_t)k;
    seq_t r_seq = (seq_t)r;
    seq_t prod = ((seq_t)3) * k_seq * (k_seq - r_seq);  /* k_seq - r_seq may be negative */
    return prod + ((seq_t)1);
}
