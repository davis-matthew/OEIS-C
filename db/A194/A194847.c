/*
 * A194847  Write n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0; sequence gives i
 *          values.
 * https://oeis.org/A194847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i where n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0. The
 *            representation is unique by the combinatorial number system of
 *            degree 3
 * Range:     u8 n <= 83, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Validated: partial, pass, 124 terms through n = 123 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t choose2_idx(idx_t x)
{
    seq_t a = x, b = (seq_t)(x - 1);
    seq_t r = (a * b) / 2;
    return (idx_t)r;
}

static inline idx_t choose3_idx(idx_t x)
{
    seq_t a = x, b = (seq_t)(x - 1), c = (seq_t)(x - 2);
    seq_t r = (a * b * c) / 6;
    return (idx_t)r;
}

/* Largest i such that C(i,3) ≤ n.  For all n ≥ 0, i ≥ 2. */
static idx_t max_i_idx(idx_t n)
{
    idx_t low = 2, high = 2;
    while (choose3_idx(high) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (choose3_idx(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Largest j < max_i such that C(j,2) ≤ rem.  j ≥ 1 for all rem ≥ 0. */
static idx_t max_j_idx(idx_t rem, idx_t max_i)
{
    idx_t low = 1, high = max_i - 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (choose2_idx(mid) <= rem)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t i   = max_i_idx(n);
    idx_t rem = n - choose3_idx(i);

    idx_t j = max_j_idx(rem, i);
    (void)j;  /* k is not needed for the output */

    return (seq_t)i;
}
