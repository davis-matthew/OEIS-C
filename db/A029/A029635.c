/*
 * A029635  The (1,2)-Pascal triangle (or Lucas triangle) read by rows.
 * https://oeis.org/A029635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) is the flattening of the (1,2)-Pascal triangle T(n,k) with
 *            T(0,0)=2, T(n,0)=1 for n>0, and T(n,k)=C(n,k)+C(n-1,k-1)
 * Range:     u8 n <= 58, u16 n <= 179, u32 n <= 644, u64 n <= 2309,
 *            u128 n <= 5252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute binomial coefficient C(n,k) using multiplicative algorithm
   with a 128‑bit intermediate to avoid overflow for uint64_t and larger
   types. This works for all supported seq_t types; the final cast
   truncates to seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    __uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }
    return (seq_t)res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
       A simple incremental search is fast enough for n up to at least 10^6. */
    idx_t r = 0;
    idx_t start = 0;
    while (start + r + 1 <= n) {
        ++r;
        start += r;
    }
    idx_t j = n - start;            /* column index in row r */

    if (r == 0)          /* T(0,0) = 2 */
        return (seq_t)2;

    if (j == 0)          /* first element of any other row is 1 */
        return (seq_t)1;

    /* General case: T(r,j) = C(r,j) + C(r-1,j-1) */
    seq_t part1 = binom(r, j);
    seq_t part2 = binom(r - 1, j - 1);
    return part1 + part2;
}
