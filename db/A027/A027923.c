/*
 * A027923  a(n) = least k such that E{1,2,...,k} >= 1^2 + 2^2 + ... + n^2,
 *          where E = 2nd elementary symmetric function.
 * https://oeis.org/A027923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k: A000914(k+1) >= A000330(n)}
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   triangular_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangular_idx.h"

static inline seq_t sum_i2(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) * (2 * kk + 1) / 6;
}

static inline seq_t second_sym(idx_t k)
{
    seq_t S = triangular_idx(k);
    seq_t Q = sum_i2(k);
    return (S * S - Q) / 2;
}
/* i.e. return the smallest k such that the second elementary symmetric
   sum of {1,…,k+1} is at least the sum of squares 1^2+…+n^2,
   then subtract 1 to match the OEIS offset. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n ≥ 1, offset is 1 */

    /* target sum of squares 1^2 + … + n^2 */
    seq_t target = sum_i2(n);

    /* Find an upper bound for k such that second_sym(k) >= target */
    idx_t lo = 1;
    idx_t hi = n;
    while (second_sym(hi) < target)
        hi *= 2;

    /* Binary search for the minimal k */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (second_sym(mid) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }

    /* OEIS a(n) counts k-1 instead of k */
    return (seq_t)(lo - 1);
}
