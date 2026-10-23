/*
 * A101866  Array read by antidiagonals: Arnoux's product T(n,k) = n * k = nk
 *          + ceiling(phi n) ceiling(phi k), where phi = (1 + sqrt(5))/2 ; n,
 *          k >= 1.
 * https://oeis.org/A101866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) read by antidiagonals, where T(i,j) = i*j +
 *            ceil(phi*i)*ceil(phi*j), phi = (1+sqrt(5))/2, i,j ≥ 1
 * Range:     u8 n <= 126, u16 n <= 9791, u32 n <= 9791, u64 n <= 9791,
 *            u128 n <= 9791, big any n
 * Helpers:   diag_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers needed for integer square root */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/diag_idx.h"

/* 128‑bit integer sqrt (floor) using binary search. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);  /* 2^64 > sqrt(max 128‑bit) */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* ceil(phi * n) for integer n ≥ 1. */
static seq_t ceil_phi(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 prod = (unsigned __int128)5 * nn * nn; /* 5 * n^2 = (sqrt(5)*n)^2 */
    unsigned __int128 sqrt5_n = isqrt128(prod);  /* floor(n*sqrt(5)) */
    seq_t floor_phi_n = (seq_t)((nn + sqrt5_n) / 2);  /* floor(phi*n) */
    return floor_phi_n + 1;                         /* ceil(phi*n) */
}

/* Main function: sequence A101866. */
seq_t a(idx_t n)
{
    idx_t d = diag_idx(n);                       /* diagonal containing term n */
    idx_t before = (d - 1) * d / 2;              /* terms before this diagonal */
    idx_t offset = n - before - 1;               /* 0‑based offset within diagonal */

    idx_t i = offset + 1;                        /* row index */
    idx_t j = d - i + 1;                         /* column index (symmetry) */

    seq_t seq_i = (seq_t)i;
    seq_t seq_j = (seq_t)j;
    seq_t ij = seq_i * seq_j;                    /* i*j */

    seq_t cip = ceil_phi(i);                     /* ceil(phi*i) */
    seq_t cjp = ceil_phi(j);                     /* ceil(phi*j) */

    return ij + cip * cjp;                       /* T(i,j) */
}
