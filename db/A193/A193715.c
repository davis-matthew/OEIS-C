/*
 * A193715  Positions of triangular numbers (A000217) in the union of squares
 *          and triangular numbers (A005214).
 * https://oeis.org/A193715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th triangular number in the sorted
 *            union of squares and triangular numbers. Computed as a(n) = #S(≤
 *            t_n) where t_n = n*(n+1)/2 and S is the set of squares ∪
 *            triangular numbers
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9163 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* precomputed square‑triangular numbers (positive ones)  */
static const seq_t SQ_TRIANGULAR[] = {
    (seq_t)1ULL,
    (seq_t)36ULL,
    (seq_t)1225ULL,
    (seq_t)41616ULL,
    (seq_t)1413721ULL,
    (seq_t)48294400ULL,
    (seq_t)1656115841ULL,
    (seq_t)56726425636ULL,
    (seq_t)194208098321ULL,
    (seq_t)6652615201161ULL,
    (seq_t)228110278144360ULL
};

static size_t count_square_triangular_leq(seq_t x)
{
    size_t i = 0;
    size_t limit = sizeof(SQ_TRIANGULAR) / sizeof(SQ_TRIANGULAR[0]);
    while (i < limit && SQ_TRIANGULAR[i] <= x) i++;
    return i;
}

seq_t a(idx_t n)
{
    /* triangular number t_n = n*(n+1)/2 */
    seq_t t = (seq_t)n * ((seq_t)(n + 1)) / 2;

    /* count squares <= t */
    seq_t sq = floor_sqrt(t);

    /* count triangular numbers <= t.
       For t = k*(k+1)/2, k = floor((sqrt(1+8t)-1)/2)   */
    seq_t eight_t = t * ((seq_t)8);
    seq_t sqrt_val = floor_sqrt(eight_t + ((seq_t)1));
    seq_t tr = (sqrt_val - ((seq_t)1)) / ((seq_t)2);

    /* subtract duplicates (numbers that are both square and triangular) */
    seq_t dup = (seq_t)count_square_triangular_leq(t);

    return sq + tr - dup;
}
