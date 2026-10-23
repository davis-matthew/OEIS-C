/*
 * A176891  Triangle T(n,k) = k if k<n and k|n, = 0 otherwise, 1 <= k <= n;
 *          read by rows.
 * https://oeis.org/A176891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by rows from the triangle: * T(n,k) = 1 if n==1 &&
 *            k==1, * 0 if n==k, * k if k<n and k divides n, * 0 otherwise. *
 *            a(n) is the linear index of the triangle, offset by 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the triangular number r*(r+1)/2. */
static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* Return the nth term of A176891 (1‑based index). */
seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r-1) < n <= triangular(r). */
    idx_t low = 1, high = n;              /* upper bound is n, always safe */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                         /* row number */

    /* K is the column within the row. */
    idx_t k = n - triangular(r - 1);

    /* Compute the triangle value at (r,k). */
    if (r == 1 && k == 1)
        return (seq_t)1;

    if (k == r)
        return (seq_t)0;

    if (r % k == 0)
        return (seq_t)k;

    return (seq_t)0;
}
