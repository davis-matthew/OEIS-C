/*
 * A055257  Sums of two powers of 6.
 * https://oeis.org/A055257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 6^i + 6^j where * j = floor((sqrt(8*n+1)-1)/2), * i = n -
 *            j*(j+1)/2. * The sequence is the triangle T(i,j)=6^i+6^j read
 *            row‑wise with j≥i
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 89, u64 n <= 324,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Find the largest k such that k*(k+1)/2 <= n.  This k equals the
 * row index j in the triangle. */
static idx_t triangular_index(idx_t n)
{
    idx_t lo = 0, hi = 1ULL << 32;   /* enough for all uint64_t n */
    idx_t best = 0;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

/* Compute 6^exp as seq_t (arbitrary precision or 64/128 bit). */
static seq_t pow6(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 6;

    while (exp) {
        if (exp & 1LL)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* row index j */
    idx_t j = triangular_index(n);

    /* cumulative count before row j: j*(j+1)/2 */
    __uint128_t prev128 = (__uint128_t)j * (j + 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* column index i within the row */
    idx_t i = n - prev;

    return pow6(i) + pow6(j);
}
