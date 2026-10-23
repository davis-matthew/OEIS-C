/*
 * A107250  Diagonal sums of number array A107249.
 * https://oeis.org/A107250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} binom(floor((2n-3k-2)/2),
 *            floor((n-2k)/2))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        int64_t num = (int64_t)2 * (int64_t)n - (int64_t)3 * (int64_t)k - 2;
        idx_t N = (num >= 0) ? (idx_t)(num / 2) : 0;
        idx_t K = (n - 2 * k) / 2;
        sum += binom(N, K);
    }
    return sum;
}
