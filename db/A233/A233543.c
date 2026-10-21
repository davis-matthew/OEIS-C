/*
 * A233543  Triangle read by rows: T(n,k) = k!.
 * https://oeis.org/A233543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = (n - i(i+1)/2)! where i = floor((sqrt(8n+1)-1)/2)
 *            Equivalently, read the triangle T(n,k)=k! row‑by‑row; a(n) is
 *            the factorial of the column index of the element located at
 *            linear index n
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t row_from_index(idx_t n)
{
    /* Find the largest i such that i*(i+1)/2 <= n */
    idx_t lo = 0, hi = n + 1;          /* hi exclusive */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;                     /* last i that satisfies */
}

seq_t a(idx_t n)
{
    idx_t i = row_from_index(n);
    __uint128_t start128 = (__uint128_t)i * (i + 1) / 2;
    idx_t start = (idx_t)start128;     /* safe: start <= n */
    idx_t j = n - start;               /* column index */

    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= j; ++k)
        res *= (seq_t)k;               /* compute j! */

    return res;
}
