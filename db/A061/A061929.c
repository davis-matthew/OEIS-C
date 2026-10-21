/*
 * A061929  Triangle with n >= k >= 0 where a(n,k) =
 *          2^k*3^(n-k)*(C(n+1,0)+C(n+1,1)+...C(n+1,k)).
 * https://oeis.org/A061929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0}^{n} 2^k * 3^(n-k) * sum_{i=0}^{k} C(n+1, i)
 *            (triangle entry)
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that T(r) = r*(r+1)/2 <= n < T(r+1) */
    uint64_t r = 0;
    while (1) {
        uint64_t r1 = r + 1;
        uint64_t r2 = r + 2;
        unsigned __int128 tri = (unsigned __int128)r1 * r2 / 2;
        if (tri <= n)
            r++;
        else
            break;
    }

    uint64_t start = (uint64_t)r * (r + 1) / 2;
    uint64_t k = n - start;

    /* compute 2^k and 3^(r-k) */
    seq_t pow2 = 1, pow3 = 1;
    for (uint64_t i = 0; i < k; ++i)
        pow2 *= (seq_t)2;
    for (uint64_t i = 0; i < r - k; ++i)
        pow3 *= (seq_t)3;

    /* compute sum_{i=0}^{k} C(r+1,i) */
    seq_t binom = 1;          /* C(r+1,0) */
    seq_t sum = binom;
    for (uint64_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(r + 1 - (i - 1)) / (seq_t)i;
        sum += binom;
    }

    return pow2 * pow3 * sum;
}
