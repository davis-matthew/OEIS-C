/*
 * A131063  Triangle read by rows: T(n,k) = 5*binomial(n,k) - 4 for 0 <= k <=
 *          n.
 * https://oeis.org/A131063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 5*binomial(r,k) - 4 where r = floor((sqrt(8*n + 1) -
 *            1)/2) (row index) k = n - r*(r+1)/2 (column index) The triangle
 *            is read row‑by‑row
 * Range:     u8 n <= 38, u16 n <= 159, u32 n <= 575, u64 n <= 2239,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) using 128‑bit arithmetic
   to avoid intermediate overflow.  k is clamped to n-k for efficiency. */
static inline seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i;  division is exact */
        __uint128_t tmp = ( __uint128_t)res * (n - k + i);
        res = (seq_t)(tmp / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that offset(r) <= n < offset(r+1),
       where offset(r) = r*(r+1)/2.  Loop is very cheap for n<=10000. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r;                                     /* r now satisfies the inequality */

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;                    /* column index within the row */

    seq_t coeff = binom_mul(r, k);            /* compute binomial(r,k) */
    return coeff * 5 - 4;                    /* 5*coeff - 4 */
}
