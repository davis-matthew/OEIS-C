/*
 * A185730  Array by antidiagonals: T(n,k) = k*(k+1)*n*(n+1)*(k*n-n+2*k+7)/36.
 * https://oeis.org/A185730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n_term, k) where T(n,k) =
 *            k*(k+1)*n*(n+1)*(k*n-n+2*k+7)/36
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerated by antidiagonals of the array defined by T(n,k). */

#ifndef __A185730_H__
#define __A185730_H__

#include <stdint.h>
#include <stddef.h>

/* Compute T(n,k) with 0‑based indices converted to seq_t first. */
static inline seq_t
T_value(idx_t n, idx_t k)
{
    /* Cast indices to seq_t before multiplication. */
    seq_t s_n  = (seq_t)n;
    seq_t s_k  = (seq_t)k;

    seq_t t1 = s_k * (s_k + 1);          /* k*(k+1) */
    seq_t t2 = s_n * (s_n + 1);          /* n*(n+1) */
    seq_t t3 = s_k * s_n - s_n + 2 * s_k + 7; /* k*n-n+2*k+7 */

    seq_t numerator = t1 * t2;
    numerator = numerator * t3;          /* fully compute numerator */

    return numerator / 36;               /* integer division */
}

/* Find diagonal d such that (d-1)*d/2 ≥ idx, d >= 2. */
static inline idx_t
find_diagonal(idx_t idx)
{
    /* Upper bound by doubling until condition satisfied. */
    idx_t lo = 1, hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < idx) {
        hi <<= 1;
    }

    /* Binary search between lo and hi. */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= idx) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;  /* hi is the minimal d satisfying the inequality */
}

/* Return the n‑th term of OEIS A185730. */
seq_t a(idx_t n)
{
    /* Sequence is 1‑based.  n ≥ 1 by spec. */
    if (n == 0) {
        /* undefined, but return 0 to avoid UB */
        return 0;
    }

    /* Determine diagonal d and offset within diagonal. */
    idx_t d = find_diagonal(n);          /* d ≥ 2 */
    idx_t prev = 0;
    if (d >= 3) {
        prev = (unsigned __int128)(d - 2) * (d - 1) / 2; /* previous triangular */
    }

    idx_t offset = n - prev - 1;         /* 0‑based offset on this diagonal */

    idx_t n_term = offset + 1;           /* row index (original n)  */
    idx_t k_term = d - n_term;           /* column index (original k) */

    return T_value(n_term, k_term);
}

#endif /* __A185730_H__ */
