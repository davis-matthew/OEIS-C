/*
 * A205813  Triangle T(n,k), read by rows, given by (0, 2, 1, 1, 1, 1, 1, 1,
 *          1, ...) DELTA (1, 0, 0, 0, 0, 0, 0, 0, ...) where DELTA is the
 *          operator defined in A084938.
 * https://oeis.org/A205813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the (n+1)-st element of the triangle T(n,k), * read by
 *            rows: T(n,0), T(n,1), …, T(n,n) for n = 0,1,2,... * T(n,k) =
 *            [x^n] (x / sqrt(1-4x))^k * For even k = 2p: T(n,2p) =
 *            binom(n-p-1, n-2p) * 4^(n-2p) * For odd k = 2p+1: T(n,2p+1) =
 *            2^(n-k) / (n-k)! * * ∏_{j=1}^{n-k} (2(p+j)-1)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/seq_pow.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t r = find_row(n);                     /* row index */
    unsigned __int128 tmp = (unsigned __int128)r * (r + 1) / 2;
    idx_t row_start = (idx_t)tmp;
    idx_t k = n - row_start;                   /* column index in row r */

    if (k == r) return (seq_t)1;               /* last element of a row */
    if (k == 0) return (seq_t)0;               /* first element, except row 0 */

    idx_t m = r - k;                           /* m = n - k */

    if ((k & 1) == 0) {                       /* k even: k = 2p  */
        idx_t p = k / 2;
        idx_t bin_n = r - p - 1;
        idx_t bin_k = m;
        seq_t bin = binomial(bin_n, bin_k);
        seq_t pow4 = seq_pow((seq_t)4, m);
        return bin * pow4;
    } else {                                   /* k odd:  k = 2p+1 */
        idx_t p = (k - 1) / 2;
        seq_t prod = (seq_t)1;
        for (idx_t j = 1; j <= m; ++j) {
            seq_t term = (seq_t)(2 * (p + j) - 1);   /* odd factor */
            prod *= term;
        }
        seq_t pow2m = seq_pow((seq_t)2, m);
        seq_t numer = prod * pow2m;
        seq_t denom = factorial(m);
        return numer / denom;
    }
}
