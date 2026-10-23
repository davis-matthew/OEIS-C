/*
 * A157174  Triangle, read by rows, T(n,k,m) = (m*(n-k)+1)*binomial(n-1, k-1)
 *          + (m*k+1)* binomial(n-1, k) - m*k*(n-k)*binomial(n-2, k-1), with m
 *          = 3.
 * https://oeis.org/A157174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c,m=3) where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r*(r+1)/2, and T(r,c,m) = (m*(r-c)+1)*C(r-1,c-1) +
 *            (m*c+1)*C(r-1,c) - m*c*(r-c)*C(r-2,c-1) (C is binomial
 *            coefficient)
 * Range:     i8 n <= 47, i16 n <= 110, i32 n <= 417, i64 n <= 1735,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: floor of integer row index for a given linear index n. */
static idx_t _row_of(idx_t n)
{
    /* Binary search for the largest r such that r*(r+1)/2 <= n. */
    idx_t lo = 0;
    idx_t hi = n;                 /* hi is surely large enough */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 offset = (unsigned __int128)mid * (mid + 1) / 2;
        if (offset <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Handle the very first element separately. */
    if (n == 0)
        return (seq_t)1;

    const seq_t M = 3;             /* m = 3 */

    idx_t r = _row_of(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;          /* 0 <= c <= r */

    /* Special case for the first row (r == 0). */
    if (r == 0)
        return (seq_t)1;

    seq_t part1 = 0, part2 = 0, part3 = 0;

    /* First term: (m*(r-c)+1)*C(r-1,c-1) */
    if (c >= 1) { /* C(r-1,c-1) defined */
        seq_t coeff = (M * (seq_t)(r - c)) + (seq_t)1;
        part1 = coeff * binomial(r - 1, c - 1);
    }

    /* Second term: (m*c+1)*C(r-1,c) */
    if (c <= r - 1) { /* C(r-1,c) defined */
        seq_t coeff = (M * (seq_t)c) + (seq_t)1;
        part2 = coeff * binomial(r - 1, c);
    }

    /* Third term: -m*c*(r-c)*C(r-2,c-1) */
    if (r >= 2 && c >= 1 && c - 1 <= r - 2) { /* C(r-2,c-1) defined */
        seq_t coeff = M * (seq_t)c * (seq_t)(r - c);
        part3 = coeff * binomial(r - 2, c - 1);
    }

    return part1 + part2 - part3;
}
