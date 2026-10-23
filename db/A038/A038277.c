/*
 * A038277  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*11^j.
 * https://oeis.org/A038277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 47, u64 n <= 144, u128 n <= 542,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038277
   a(n) = T(i,k) where T(i,j)=C(i,j)*7^(i-j)*11^j
   i = floor((sqrt(8*n+1)-1)/2)   (row)
   k = n - i*(i+1)/2              (column)   */

/* Helper: integer square root of an unsigned 64‑bit value (floor). */
static inline idx_t _floor_isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid > 0 && mid > x / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t temp = 8 * n + 1;          /* 8*n+1 is safe for n up to ~10^9 in idx_t */
    idx_t s     = _floor_isqrt_u64(temp);
    idx_t i     = (s - 1) / 2;       /* row index */
    idx_t k     = n - (i * (i + 1)) / 2; /* column index */

    seq_t bin   = binomial(i, k);
    seq_t pow7  = big_pow((seq_t)7, (seq_t)(i - k));
    seq_t pow11 = big_pow((seq_t)11, (seq_t)k);

    return bin * pow7 * pow11;
}
