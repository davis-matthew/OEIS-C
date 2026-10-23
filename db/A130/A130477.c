/*
 * A130477  T(n,k) is the number of permutations of [n] with maximum descent
 *          k, T(n,k) for n >= 0 and 0 <= k <= n, triangle read by rows.
 * https://oeis.org/A130477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(1+8*n)-1)/2), c = n -
 *            r*(r+1)/2 and T(n,k) = 1 if k == 0 (n-k+1) * Π_{i=n-k+3}^{n+1} i
 *            otherwise (0^k is 1 only when k == 0)
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrtl */

static inline idx_t _row_from_index(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    long double d = sqrtl((long double)8.0L * n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) * 0.5L);

    /* Adjust for possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = _row_from_index(n);
    idx_t c = n - r * (r + 1) / 2;   /* column index within the row */

    /* T(r,c) */
    if (c == 0) return (seq_t)1;     /* by definition */

    seq_t factor = (seq_t)(r - c + 1);
    seq_t ratio  = 1;

    /* product from (r - c + 3) to (r + 1) inclusive */
    idx_t start = r - c + 3;
    idx_t end   = r + 1;
    for (idx_t i = start; i <= end; ++i)
        ratio *= (seq_t)i;

    return factor * ratio;
}
