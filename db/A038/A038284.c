/*
 * A038284  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*6^j.
 * https://oeis.org/A038284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 47, u64 n <= 175, u128 n <= 639,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038284: a(n) = binomial(m,k)*8^(m-k)*6^k,
   where m = floor((sqrt(8*n+1)-1)/2) and k = n - m*(m+1)/2.
   The sequence is obtained by reading the triangle row‑wise.
*/

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t floor_sqrt_idx(idx_t n)
{
    /* integer floor of sqrt(n) using binary search and 128‑bit multiply */
    idx_t high = 1;
    while ((__uint128_t)high * high <= n) high <<= 1;
    idx_t low = high >> 1;
    while (low < high - 1) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* compute parameters m and k for the n-th element */
    idx_t t = 8 * n + 1;
    idx_t s = floor_sqrt_idx(t);
    idx_t m = (s - 1) / 2;
    idx_t triangular = (idx_t)(((__uint128_t)m * (m + 1)) / 2);
    idx_t k = n - triangular;

    /* compute the binomial coefficient */
    seq_t val = binomial(m, k);

    /* compute 8^(m-k) */
    seq_t pow8 = 1;
    idx_t exp = m - k;
    seq_t base8 = 8;
    while (exp > 0) {
        if (exp & 1) pow8 *= base8;
        base8 *= base8;
        exp >>= 1;
    }

    /* compute 6^k */
    seq_t pow6 = 1;
    exp = k;
    seq_t base6 = 6;
    while (exp > 0) {
        if (exp & 1) pow6 *= base6;
        base6 *= base6;
        exp >>= 1;
    }

    val *= pow8;
    val *= pow6;
    return val;
}
