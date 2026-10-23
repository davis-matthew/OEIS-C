/*
 * A191348  Array read by antidiagonals: ((ceiling(sqrt(n)) + sqrt(n))^k +
 *          (ceiling(sqrt(n)) - sqrt(n))^k)/2 for columns k >= 0 and rows n >=
 *          0.
 * https://oeis.org/A191348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite table * T(n,0) = 1 *
 *            T(n,1) = ceil(sqrt(n)) * for k >= 2: * T(n,k) = T(n,k-2)*(n -
 *            T(n,1)^2) + T(n,k-1)*T(n,1)*2 * and listing the entries by
 *            antidiagonals (rows n≥0, columns k≥0). * The linear index n
 *            corresponds to the element of sum s = n+k * satisfying...
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of sqrt for 64‑bit unsigned values.
 * Uses only integer arithmetic and protects against overflow. */
static inline idx_t floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Integer ceil of sqrt: one more than floor unless n is a perfect square. */
static inline idx_t ceil_sqrt(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t f = floor_sqrt(n);
    return (f * f == n) ? f : f + 1;
}

seq_t a(idx_t n)
{
    /* Determine the antidiagonal sum s such that
     * s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Row index i and column index j inside that row:
     *   i = n - s*(s+1)/2
     *   j = s - i  */
    idx_t cumulative_before = s * (s + 1) / 2;
    idx_t i = n - cumulative_before;
    idx_t j = s - i;

    /* Handle the first two columns directly */
    if (j == 0)
        return (seq_t)1;
    if (j == 1)
        return (seq_t)ceil_sqrt(i);

    /* General case: compute T(i, j) via recurrence */
    idx_t C = ceil_sqrt(i);
    seq_t t0 = (seq_t)1;            /* T(i,0) */
    seq_t t1 = (seq_t)C;            /* T(i,1) */
    seq_t diff = (seq_t)i - (seq_t)C * (seq_t)C; /* i - C^2 */

    for (idx_t k = 2; k <= j; ++k) {
        seq_t tmp = t0 * diff + t1 * ((seq_t)C * 2);
        t0 = t1;
        t1 = tmp;
    }
    return t1;
}
