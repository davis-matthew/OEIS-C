/*
 * A204179  Symmetric matrix based on f(i,j) defined by f(i,1)=f(1,j)=1;
 *          f(i,i)= i; f(i,j)=0 otherwise; by antidiagonals.
 * https://oeis.org/A204179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = M(i,j), where M satisfies M(i,1)=M(1,j)=1, M(i,i)=i,
 *            otherwise 0; (i,j) is the n‑th cell in the anti‑diagonal
 *            traversal of the matrix M starting at (1,1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt128(__uint128_t d)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 34; /* > sqrt(8*2^64+1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= d)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static uint64_t ceil_triangular(idx_t n)
{
    if (n == 0) return 0;
    __uint128_t disc = (__uint128_t)1 + (__uint128_t)8 * n;
    uint64_t sqrt_disc = isqrt128(disc);
    uint64_t m = (1 + sqrt_disc) / 2;
    /* adjust up as needed */
    while ((__uint128_t)m * (m - 1) / 2 < n)
        ++m;
    return m;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) return (seq_t)0;   /* defensive, not used */
    uint64_t m = ceil_triangular(n);
    idx_t prev = ((idx_t)(m - 1) * (m - 2)) / 2;
    idx_t offset = n - prev - 1;
    idx_t i = offset + 1;          /* 1 ≤ i ≤ m-1 */
    idx_t j = m - i;               /* 1 ≤ j ≤ m-1 */

    if (i == 1 || j == 1)
        return (seq_t)1;
    if (i == j)
        return (seq_t)i;
    return (seq_t)0;
}
