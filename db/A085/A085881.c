/*
 * A085881  Triangle T(n,k) read by rows: multiply row n of Pascal's triangle
 *          (A007318) by A001147(n).
 * https://oeis.org/A085881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r,k) * (2r-1)!!, where r = ⌊(√(8n+1)-1)/2⌋, k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 124,
 *            u128 n <= 356, big any n
 * Helpers:   odd_double_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_double_factorial.h"

/* compute binomial coefficient C(n,k) using multiplicative formula */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;        /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)0;
    while ((r + (idx_t)1) * (r + (idx_t)2) / (idx_t)2 <= n)
        ++r;

    /* column index within the row */
    idx_t offset = r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = n - offset;

    /* compute binomial(r,k) * (2r-1)!! */
    seq_t bin = binom(r, k);
    seq_t fact = odd_double_factorial(r);
    return bin * fact;
}
