/*
 * A174730  Triangle read by rows: T(n,k) = (1-4^n) * Eulerian(n + 1, m) +
 *          4^n.
 * https://oeis.org/A174730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) = (1-4^r) * Eulerian(r+1,k) + 4^r * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2 * (triangle T(r,k)
 *            read by rows with offset 0)
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 38, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

static inline idx_t
row_of(idx_t n)
{
    idx_t lo = 0, hi = n + 1;               /* hi is exclusive upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Eulerian(N,k) for 0 <= k < N,  N >= 1 */
static inline seq_t
eulerian_row_k(idx_t N, idx_t k)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t bin = binomial(N + 1, i);
        seq_t powv = pow_seq((seq_t)(k + 1 - i), (int)N);
        seq_t term = bin * powv;
        if (i & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t eul = eulerian_row_k(r + 1, k);
    seq_t pow4 = pow_seq((seq_t)4, (int)r);
    seq_t tmp = (seq_t)1 - pow4;
    tmp *= eul;
    tmp += pow4;
    return tmp;
}
