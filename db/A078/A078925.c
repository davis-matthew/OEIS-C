/*
 * A078925  Triangle of T1(n,m) = number of bracelets (necklaces that can be
 *          turned over) with m white beads and (2n+1-m) black ones, for
 *          1<=m<=n.
 * https://oeis.org/A078925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T1(r, c) where r(c) is the row and column extracted from
 *            the linear index n. T1(r, c) counts bracelets of length 2r+1
 *            with c white beads
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */
#include <stdint.h>

/* Compute Euler's totient function φ(x).  x ≤ 20000 here, so
   a simple trial division suffices. */
static inline seq_t phi_u64(uint64_t x)
{
    seq_t res = (seq_t)x;
    uint64_t t = x;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            res -= res / (seq_t)p;
        }
    }
    if (t > 1) res -= res / (seq_t)t;
    return res;
}

/* Find the row number r such that r(r+1)/2 ≥ idx.  idx ≥ 1. */
static inline idx_t find_row(idx_t idx)
{
    idx_t low = 1, high = idx;   /* upper bound never too large */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= idx) high = mid;
        else            low = mid + 1;
    }
    return low;
}

/* Compute the bracelet number T1(row, col) for odd length L = 2*row+1. */
static seq_t compute_T1(idx_t row, idx_t col)
{
    /* L is odd, n_half = L/2 = row. */
    idx_t L      = (row << 1) + 1;   /* 2*row+1 */
    idx_t n_half = row;

    seq_t rotation_sum = 0;

    /* Sum over all divisors t of L contributing to rotation fixes. */
    for (idx_t d = 1; d * d <= L; ++d) {
        if (L % d != 0) continue;

        /* First divisor d */
        idx_t t1 = d;
        if (col % t1 == 0) {
            idx_t cycles = L / t1;          /* number of cycles */
            idx_t ones   = col / t1;          /* ones per cycle */
            seq_t bin    = binom(cycles, ones);
            seq_t phi    = phi_u64(t1);
            rotation_sum += phi * bin;
        }

        /* Second divisor d2 = L/d, distinct from d. */
        idx_t d2 = L / d;
        if (d2 != d) {
            idx_t t2 = d2;
            if (col % t2 == 0) {
                idx_t cycles = L / t2;
                idx_t ones   = col / t2;
                seq_t bin    = binom(cycles, ones);
                seq_t phi    = phi_u64(t2);
                rotation_sum += phi * bin;
            }
        }
    }

    /* Reflection contribution.  For odd L there are L reflections,
       each with one fixed bead and (L-1)/2 pairs. */
    seq_t even_part = 0, odd_part = 0;
    if ((col & 1) == 0) {                 /* m even */
        even_part = binom(n_half, col / 2);
    } else {                               /* m odd */
        odd_part = binom(n_half, (col - 1) / 2);
    }
    seq_t reflection_sum = (seq_t)L * (even_part + odd_part);

    /* Total bracelets: (rotation_sum + reflection_sum) / (2*L). */
    seq_t denom  = (seq_t)2 * (seq_t)L;
    seq_t total  = (rotation_sum + reflection_sum) / denom;
    return total;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for out-of-range */

    idx_t row = find_row(n);
    idx_t row_start = (row - 1) * row / 2; /* first index of this row (1‑based) */
    idx_t col = n - row_start;            /* 1‑based column within the row */

    return compute_T1(row, col);
}
