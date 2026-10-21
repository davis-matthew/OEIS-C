/*
 * A221645  Square array read by antidiagonals: T(n,k^2) = A040000(n) (=
 *          1,2,2,2,...) if n=0 (mod k), T(n,k) = 0 else, n>=0, k>=1.
 * https://oeis.org/A221645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) by
 *            anti‑diagonals, where T(n,k) = A040000(n) if k is a perfect
 *            square and n ≡ 0 (mod √k) T(n,k) = 0 otherwise A040000(0) = 1
 *            and A040000(n>0) = 2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t floor_isqrt_u64(idx_t n)
{
    /* integer square root: floor(sqrt(n)) */
    idx_t low = 0, high = (1ULL << 32);        /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

static inline idx_t triangular(idx_t d)
{
    return (d * (d + 1)) / 2;
}

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = (floor_isqrt_u64(8 * n + 1) - 1) >> 1;
    if (triangular(d) < n)
        ++d;

    /* position inside that diagonal */
    idx_t start = triangular(d - 1) + 1;      /* first index on diagonal d */
    idx_t t = n - start;                      /* 0‑based offset in the diagonal */
    idx_t i = d - 1 - t;                      /* row index (0‑based) */
    idx_t j = t + 1;                          /* column index (1‑based) */

    /* evaluate T(i,j) */
    idx_t s = floor_isqrt_u64(j);
    if (s * s != j)                 /* column not a perfect square */
        return (seq_t)0;

    if ((i % s) == 0) {                 /* n ≡ 0 (mod s) */
        return (i == 0) ? (seq_t)1 : (seq_t)2;
    }
    return (seq_t)0;
}
