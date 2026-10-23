/*
 * A134520  Triangle read by rows: A007318 + A128174 - A000012 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A134520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the lower triangular array *
 *            T(r,c) = C(r,c) - ((r%2)!=(c%2)), with 0 ≤ c ≤ r. * The mapping
 *            from linear index n to (r,c) uses the triangular number: * r =
 *            max{ r : r(r+1)/2 ≤ n } * c = n - r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until the triangle number exceeds n */
    while (( (__uint128_t)high * (high + 1) / 2 ) <= n)
        high <<= 1;

    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Computes the binomial coefficient C(n,k) in seq_t, using multiplicative form. */
static seq_t binom_choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t offset = (idx_t)(( (__uint128_t)r * (r + 1) ) / 2);
    idx_t c = n - offset;
    seq_t bin = binom_choose(r, c);
    if ( ((r ^ c) & 1) != 0 )                     /* parity differs */
        return bin - (seq_t)1;
    else
        return bin;
}
