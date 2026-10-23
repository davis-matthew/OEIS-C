/*
 * A173755  Table read by rows, T(n,k) = (-1)^(n-k)*2^(2*k-bw(k)), where bw(k)
 *          is the binary weight of k (A000120).
 * https://oeis.org/A173755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(r,k) = (-1)^(r-k)*2^(2*k-popcount(k)) where r is the
 *            unique non‑negative integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 *            and k = n - r*(r+1)/2 is the column index in row r
 * Range:     i8 n <= 13, i16 n <= 43, i32 n <= 151, i64 n <= 559,
 *            i128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
       Using a small loop – r will be at most ≈√(2n) for n up to 10 000
       (≤ 200 for n = 100 000), so the loop is negligible. */
    idx_t r = 0;
    while (((seq_t)(r + 1) * (r + 2) / 2) <= n)
        r++;

    /* Column index within the row */
    idx_t base = r * (r + 1) / 2;
    idx_t k = n - base;

    /* Compute the exponent: 2*k - popcount(k) */
    idx_t pop = __builtin_popcountll((unsigned long long)k);
    idx_t exp = 2 * k - pop;

    /* Base value 2^exp */
    seq_t val = pow2(exp);

    /* Apply the sign factor: (-1)^(r-k) */
    if (((r - k) & 1ULL) != 0)
        val = -val;

    return val;
}
