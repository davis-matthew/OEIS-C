/*
 * A062110  A(n,k) is the coefficient of x^k in (1-x)^n/(1-2*x)^n for n, k >=
 *          0; Table A read by descending antidiagonals.
 * https://oeis.org/A062110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 122, u32 n <= 383, u64 n <= 1286,
 *            u128 n <= 4774, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062110:   a(n) = coefficient of x^n in the square array
 * A(n,k) read along descending antidiagonals.
 * Recurrence for A(n,k):
 *    A(0,0)=1,   A(0,k)=0 for k>0,   A(n,0)=1
 *    A(n,k)=2*A(n,k-1)+A(n-1,k)-A(n-1,k-1)   for n,k>=1
 * The mapping from the global index n to array coordinates:
 *    s   = floor( (sqrt(8*n+1)-1)/2 )   // index of antidiagonal
 *    t   = s*(s+1)/2                   // terms before this diagonal
 *    i   = n - t                       // row index
 *    k   = s - i                       // column index
 * Then a(n) = A(i,k).
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <stdint.h>
#else
#include <cstdlib>
#include <stdint.h>
#endif

/* Allocation helpers that work in C and C++ */
#ifndef __cplusplus
#define NEW(seq_t, sz) ((seq_t*)calloc((sz), sizeof(seq_t)))
#define FREE(ptr)      free(ptr)
#else
#define NEW(seq_t, sz) new seq_t[(sz)]
#define FREE(ptr)      delete[] (ptr)
#endif

/* Integer square root using 128‑bit intermediate to avoid overflow */
static inline idx_t isqrt_uint128(idx_t x)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 xx = x;
    idx_t r = 0, bit = ((idx_t)1) << 63;
    while (bit) {
        idx_t nxt = r | bit;
        if ((unsigned __int128)nxt * nxt <= xx) r = nxt;
        bit >>= 1;
    }
    return r;
#else
    /* Fallback: binary search using 128‑bit multiplication */
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod == x) return mid;
        if (prod < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
#endif
}

seq_t a(idx_t n)
{
    /* Find diagonal index s */
    idx_t s = 0;
    while (1) {
        idx_t t = s + 1;
        /* t*(t+1)/2 = (s+1)*(s+2)/2 might overflow, use 128‑bit */
        unsigned __int128 tri = (unsigned __int128)t * (s + 2) / 2;
        if (tri > n) break;
        ++s;
    }

    /* Coordinates on the antidiagonal */
    idx_t before = (s * (s + 1)) / 2;
    idx_t offset = n - before;
    idx_t i = offset;           /* row   */
    idx_t k = s - offset;       /* column */

    /* Allocate table A[0..s][0..s] */
    idx_t dim = s + 1;
    seq_t *A = NEW(seq_t, dim * dim);   /* zero‑initialized */

    /* Base cases */
    A[0] = (seq_t)1;                /* A(0,0)=1 */

    /* Fill table */
    for (idx_t row = 1; row <= s; ++row) {
        size_t base = row * dim;
        A[base] = (seq_t)1;          /* A(row,0)=1 */
        for (idx_t col = 1; col <= s; ++col) {
            seq_t left   = A[base + col - 1];            /* A(row,col-1) */
            seq_t up     = A[(row - 1) * dim + col];     /* A(row-1,col) */
            seq_t upl    = A[(row - 1) * dim + col - 1];  /* A(row-1,col-1) */

            seq_t val = left + left;   /* 2*left */
            val = val + up;
            val = val - upl;
            A[base + col] = val;
        }
    }

    seq_t res = A[i * dim + k];
    FREE(A);
    return res;
}
