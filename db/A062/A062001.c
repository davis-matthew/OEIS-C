/*
 * A062001  Table by antidiagonals of n-Stohr sequences: T(n,k) is least
 *          positive integer not the sum of at most n distinct terms in the
 *          n-th row from T(n,1) through to T(n,k-1).
 * https://oeis.org/A062001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangular array T(n,k) * where
 *            T(n,k) = A(n,k) and * A(n,k) = 2^(k-1) if k <= n+1 *
 *            (2^n-1)*(k-n)+1 otherwise. * The sequence a(n) enumerates the
 *            array by antidiagonals, * i.e. for n≥1 let d be the smallest
 *            integer with d*(d+1)/2 ≥ n, * let p = n - d*(d-1)/2 (1 ≤ p...
 * Range:     u8 n <= 125, u16 n <= 509, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find diagonal index d such that d*(d+1)/2 >= n */
    idx_t d = 0;
    __uint128_t cum = 0;

    while (cum < n) {
        d++;
        cum += d;
    }
    /* cum is d*(d+1)/2, previous cumulative is cum - d */
    idx_t prev_cum = (idx_t)(cum - d);
    idx_t p = n - prev_cum;            /* 1‑based position within diagonal */
    idx_t i = p;                      /* row index of T */
    idx_t j = d - p + 1;              /* column index of T */

    /* Compute A(i,j) according to the closed form */
    if (j <= i + 1) {
        return pow2(j - 1);            /* 2^(j-1) */
    } else {
        seq_t two_i  = pow2(i);        /* 2^i */
        seq_t coeff  = two_i - 1;      /* 2^i - 1 */
        seq_t diff   = (seq_t)(j - i); /* k - n in formula */
        seq_t val    = coeff * diff;   /* (2^i -1)*(j-i) */
        return val + 1;
    }
}
