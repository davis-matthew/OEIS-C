/*
 * A061177  Coefficients of polynomials ( (1 -x +sqrt(x))^(n+1) - (1 -x
 *          -sqrt(x))^(n+1) )/(2*sqrt(x)).
 * https://oeis.org/A061177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 124, i32 n <= 445, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061177
 *  a(n) = coefficient of x^k in ( (1 -x +sqrt(x))^(n+1) - (1 -x -sqrt(x))^(n+1) )/(2*sqrt(x))
 *  where k is the index of a(n) in the row-major enumeration of the triangle
 *  T(m,k).  The implementation uses the combinatorial formula:
 *
 *     T(m,k) = Sum_{j=0..k} (-1)^(k-j) * binomial(m+1,2j+1) * binomial(m-2j,k-j)
 *  for 0 ≤ k ≤ floor(m/2); for the other half of the row the symmetry
 *  T(m,k) = (-1)^m * T(m,m-k) is used.
 *
 *  The function below computes a(n) in O(n) time (actually O(row) per call)
 *  with exact arithmetic via binomial(), which is part of the utilities
 *  framework.  No dynamic allocation or floating‑point arithmetic is used.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Find the largest row m such that the cumulative number of terms up to
 * and including row m is ≤ n.  The cumulative count of terms up to row m
 * is m*(m+1)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it exceeds the desired cumulative count. */
    while (high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search for the correct row. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Triangle value T(m,k).  Recursively use symmetry to keep k within
 * the first half of the row, which guarantees a small number of terms in
 * the summation.  All arithmetic is performed on seq_t, which may be
 * an arbitrary‑precision integer. */
static seq_t T(idx_t m, idx_t k)
{
    if (k > m)
        return 0;

    /* Use symmetry for the right half of the row. */
    if (k > m - k) {
        seq_t val = T(m, m - k);
        if (m & 1)          /* (-1)^m for odd m */
            val = -val;
        return val;
    }

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        idx_t two_j_plus1 = 2 * j + 1;
        seq_t term = binomial(m + 1, two_j_plus1);
        term *= binomial(m - 2 * j, k - j);
        if ((k - j) & 1)   /* (-1)^(k-j) */
            term = -term;
        sum += term;
    }
    return sum;
}

/* External interface required by the build system. */
seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    /* Cumulative count of terms up to the row before 'row'. */
    idx_t row_start = row * (row + 1) / 2;
    idx_t k = n - row_start;
    return T(row, k);
}
