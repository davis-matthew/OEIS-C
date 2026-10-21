/*
 * A239126  Rectangular array showing the starting values M(n, k), k >= 1, for
 *          the Collatz operation (ud)^n, n >= 1, ending in an odd number,
 *          read by antidiagonals.
 * https://oeis.org/A239126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the rectangular array *
 *            M(n,k)=2^(n+1)*k−1 along its antidiagonals
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0 || x == 1) return x;
    /* use long double sqrt for initial estimate */
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    /* adjust to exact floor */
    while ((unsigned __int128)(r+1)*(r+1) <= x) ++r;
    while ((unsigned __int128)r*r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* n is the 1‑based index in the sequence */
    idx_t t = n;

    /* find the antidiagonal sum s such that (s-1)*s/2 >= t */
    idx_t N = 1 + 8 * t;
    idx_t r = isqrt_u64(N);
    if ((unsigned __int128)r * r < N) ++r;          /* ceiling of sqrt */
    idx_t s = (r + 2) / 2;                          /* sum of indices (n+k) */

    /* number of elements before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* 0‑based offset within the diagonal */
    idx_t offset = t - prev - 1;

    /* coordinates inside the diagonal:
     * order is k decreasing from s‑1 to 1, n increasing from 1 upwards */
    idx_t k      = s - offset - 1;   /* 1 <= k <= s-1 */
    idx_t n_row  = offset + 1;       /* 1 <= n_row <= s-1 */

    /* compute M(n_row, k) = 2^(n_row+1) * k - 1 */
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < n_row + 1; ++i) /* 2^(n_row+1) */
        val *= 2;

    val *= k;        /* *k */
    val -= 1;        /* -1 */

    return val;
}
