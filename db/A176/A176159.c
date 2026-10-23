/*
 * A176159  Triangle T(n,m) = (1 + binomial(n, m))^n, n>=0, 0<=m<=n.
 * https://oeis.org/A176159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1 + binomial(r, c))^r where r and c are the * row and
 *            column indices of the flat index n in the triangle * T(n,m) = (1
 *            + binomial(n,m))^n, n ≥ 0, 0 ≤ m ≤ n. * T(0,0) = 1, T(1,0)=2,
 *            T(1,1)=2, T(2,0)=4, T(2,1)=9, T(2,2)=4,
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */

/* Multiply a base by itself exp times.  Works for all seq_t types
 * that support the operators +, -, *, /, %, <<, >>, &, |, ^. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* Return the triangular number tri(r) = r*(r+1)/2 as a 128‑bit value
 * to avoid overflow when r is large.  r is an idx_t (unsigned 64‑bit). */
static inline __uint128_t triangular128(idx_t r)
{
    return ((__uint128_t)r * (r + 1)) / 2;
}

/* Find the row r such that the flat index n lies in row r.
 * Row r starts at index triangular128(r) and ends at triangular128(r+1)-1.
 */
static inline idx_t find_row(idx_t n) {
    idx_t r = 0;
    /* linear search is fine: r will be at most about 140 for n ≤ 10000. */
    while (triangular128(r + 1) <= n) {
        ++r;
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                     /* row */
    idx_t start = (idx_t)triangular128(r);     /* first index of row r */
    idx_t c = n - start;                       /* column within row r */

    seq_t bin = binomial(r, c);                /* binomial coefficient */
    seq_t base = bin + (seq_t)1;               /* 1 + binom */
    return pow_seq(base, r);                   /* (1 + binom)^r */
}
