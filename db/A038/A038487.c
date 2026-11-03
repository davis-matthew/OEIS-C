/*
 * A038487  Sums of two distinct powers of 9.
 * https://oeis.org/A038487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^A002024(n)+9^A002262(n)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 55, u64 n <= 210, u128 n <= 820,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 9990 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002024 gives the smaller exponent and A002262 the larger exponent */
/* The exponents are enumerated first by increasing larger exponent j, */
/* then by increasing smaller exponent i.  Thus a(n) is the sum of the */
/* corresponding distinct powers of 9. */
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

/* Returns 1 iff j*(j+1)/2 >= n, using 128‑bit arithmetic to avoid overflow. */
static inline int triangular_ge(idx_t n, idx_t j)
{
    return ( (__int128)j * (j + 1) / 2 ) >= n;
}

/* Computes triangular number T_j = j*(j+1)/2; the result is guaranteed to fit */
/* in idx_t because the function is only called with j such that T_j < 2^64. */
static inline idx_t triangular(idx_t j)
{
    return (idx_t)(( (__int128)j * (j + 1) ) / 2);
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 1; return 0 for safety   */
        return 0;

    /* Binary search for the smallest j with T_j >= n  */
    idx_t low = 1;
    idx_t high = n;            /* high = n is always sufficient  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular_ge(n, mid))
            high = mid;
        else
            low = mid + 1;
    }
    idx_t j = low;                 /* larger exponent */

    /* Compute T_{j-1} to find the offset within this block */
    idx_t t_prev = (j > 0) ? triangular(j - 1) : 0;
    idx_t i = n - t_prev - 1;      /* smaller exponent */

    /* Compute 9^i and 9^j using the provided big‑integer routine */
    seq_t pow_i = ipow_u64_idx(9, i);
    seq_t pow_j = ipow_u64_idx(9, j);

    return pow_i + pow_j;
}
