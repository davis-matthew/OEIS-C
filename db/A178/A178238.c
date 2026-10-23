/*
 * A178238  Triangle read by rows: partial column sums of the triangle of
 *          natural numbers (written sequentially by rows).
 * https://oeis.org/A178238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(1+8n)-1)/2) and * T(r,k) =
 *            binomial(r+1,3) - binomial(k,3) + k*(r-k+1). * Here k = n -
 *            triangular(r-1) (1‑based column index)
 * Range:     u8 n <= 66, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* integer square root of a __uint128_t value */
static inline idx_t floor_sqrt_u128(__uint128_t x)
{
    if (x <= 1) return (idx_t)x;
    /* use long double sqrt and adjust */
    long double d = sqrtl((long double)x);
    idx_t r = (idx_t)d;
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

/* binomial(x,3) for x < 3 returns 0, otherwise x*(x-1)*(x-2)/6 */
static inline seq_t binom3(idx_t x)
{
    if (x < 3) return (seq_t)0;
    seq_t sx = (seq_t)x;
    return (sx * (sx - 1) * (sx - 2)) / (seq_t)6;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for n==0, though offset is 1 */

    /* compute row r: smallest integer with r(r+1)/2 >= n */
    __uint128_t eight_n_plus1 = (__uint128_t)8 * n + 1;
    idx_t sq = floor_sqrt_u128(eight_n_plus1);
    idx_t r = (sq - 1) >> 1;            /* floor of the root */
    /* check if we need to bump r to satisfy the inequality */
    __uint128_t tri_r = (__uint128_t)r * (r + 1) / 2;
    if (tri_r < n) ++r;

    /* column index k (1‑based) */
    __uint128_t triangular_prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - triangular_prev);

    /* compute T(r,k) */
    seq_t part1 = binom3(r + 1) - binom3(k);
    seq_t part2 = (seq_t)k * ( (seq_t)(r - k + 1) );

    return part1 + part2;
}
