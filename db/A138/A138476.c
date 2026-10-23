/*
 * A138476  Triangle read by rows: expansion of (1 + 3*x^2)/(1 - x*(2*y-x)).
 * https://oeis.org/A138476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the flattened triangle coefficient of (1+3*x^2)/(1 -
 *            x*(2*y-x))
 * Range:     i8 n <= 34, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   pow2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_seq.h"

/* helper: binomial coefficient C(N,K) in seq_t */
static seq_t binom_seq(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    return res;
}

/* helper: integer power of two in seq_t */

/* helper: compute the row index (r) for linear index n
   satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t row_from_n(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high == 0) break;          /* overflow guard */
    }
    /* binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Find (row r, column k) in the triangle */
    idx_t r = row_from_n(n);
    idx_t prefix = r * (r + 1) / 2;
    idx_t k = n - prefix;

    seq_t result = 0;
    seq_t pow2 = pow2_seq(k);  /* 2^k */

    /* First contribution (m1 = (r+k)/2) */
    if (((r + k) & 1) == 0) {
        idx_t m1 = (r + k) / 2;
        if (m1 >= k) {
            seq_t bin = binom_seq(m1, k);
            seq_t term = bin * pow2;
            if (((m1 - k) & 1) != 0) term = -term;
            result += term;
        }
    }

    /* Second contribution (m2 = (r+k-2)/2) with factor 3 */
    if (((r + k) & 1) == 0 && (r + k) >= 2) {
        idx_t m2 = (r + k - 2) / 2;
        if (m2 >= k) {
            seq_t bin = binom_seq(m2, k);
            seq_t term = bin * pow2;
            if (((m2 - k) & 1) != 0) term = -term;
            term = term * 3;
            result += term;
        }
    }

    return result;
}
