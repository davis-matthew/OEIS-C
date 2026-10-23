/*
 * A128444  Array T by antidiagonals: T(n,k)=Floor(k*3^(n/2)).
 * https://oeis.org/A128444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(col * 3^(row/2)) where (row,col) are obtained by *
 *            reading the infinite matrix T(n,k)=floor(k*3^(n/2)) along
 *            antidiagonals. * Offset: 1 (a(1) is the first term listed below)
 * Range:     u8 n <= 53, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   sqrt_floor_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sqrt_floor_uint128.h"

/* ---------- helper functions ---------- */

/* Compute 3^exp as unsigned 128‑bit integer */
static unsigned __int128 pow3_uint128(idx_t exp)
{
    unsigned __int128 r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 3;
    return r;
}

/* Find the diagonal d such that n ≤ d*(d+1)/2 */
static idx_t find_diag(idx_t n)
{
    idx_t lo = 1, hi = n;                 /* n ≥ 1 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n) hi = mid;
        else          lo = mid + 1;
    }
    return lo;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard anyway */

    /* find diagonal and position inside it */
    idx_t d   = find_diag(n);
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t pos  = n - (idx_t)prev - 1;     /* 0‑based inside diagonal */

    /* coordinates (row, col) in the matrix */
    idx_t row = pos + 1;
    idx_t col = d - pos;

    /* compute the value */
    if ((row & 1) == 0) {                /* even row: exact integer */
        idx_t m = row >> 1;              /* n = 2*m */
        unsigned __int128 pow3 = pow3_uint128(m);  /* 3^m */
        unsigned __int128 val  = (unsigned __int128)col * pow3;
        return (seq_t)val;               /* fits if final result fits seq_t */
    } else {                             /* odd row: floor(k * sqrt(3) * 3^m) */
        idx_t m     = row >> 1;          /* n = 2*m + 1 */
        unsigned __int128 pow3 = pow3_uint128(m);
        unsigned __int128 k     = (unsigned __int128)col;
        unsigned __int128 p     = k * pow3;          /* k * 3^m */
        unsigned __int128 X     = p * p * 3;        /* 3 * p^2 */
        unsigned __int128 root  = sqrt_floor_uint128(X);
        return (seq_t)root;                                /* floor value */
    }
}
