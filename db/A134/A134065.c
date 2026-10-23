/*
 * A134065  Triangle read by rows: A134059 + A124927 - A007318 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A134065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=1; for n>0 let r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2. If c==0 then a(n)=3 else a(n)=4 * C(r,c) where C is
 *            the binomial coefficient
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern idx_t _floor_isqrt_u64(idx_t n); /* declared in utils, may be unused but present */

static seq_t binom(idx_t r, idx_t k)
{
    if (k > r) return (seq_t)0;
    if (k == 0 || k == r) return (seq_t)1;
    if (k > r - k) k = r - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t mult = (seq_t)r - (seq_t)k + (seq_t)i;   /* r-k+i */
        res = (res * mult) / (seq_t)i;                /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n + 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * (seq_t)(mid + 1) / 2;
        if (tri <= (seq_t)n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    seq_t tri = (seq_t)r * (seq_t)(r + 1) / 2;
    idx_t c = n - r * (r + 1) / 2;

    if (c == 0) return (seq_t)3;
    return (seq_t)4 * binom(r, c);
}
