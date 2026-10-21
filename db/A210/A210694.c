/*
 * A210694  T(n,k)=Number of (n+1)X(n+1) -k..k symmetric matrices with every
 *          2X2 subblock having sum zero.
 * https://oeis.org/A210694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 41, u32 n <= 128, u64 n <= 367,
 *            u128 n <= 1020, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210694:  
   a(n) = T(r,k) where T(n,k)=k^(n+1)+(k+1)^(n+1) and the pairs (n,k)
   are listed by anti‑diagonal enumeration (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),… */
/* For the first 24 terms (n, a(n)) are:
   1,5  2,13  3,9  4,25  5,35  6,17  7,41  8,91  9,97 10,33 11,61 12,189
   13,337 14,275 15,65 16,85 17,341 18,881 19,1267 20,793 21,129 22,113 23,559 24,1921
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Integer square‑root for uint64_t using a binary search. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined for n ≥ 1 only */

    /* Find the anti‑diagonal 's' such that (s-1)*s/2 ≥ n. */
    idx_t need = 2 * n;                          /* 2n fits in idx_t for all n considered */
    idx_t sqrt_val = isqrt_u64(1 + need);        /* floor(sqrt(1+8n)) */
    idx_t s = (1 + sqrt_val) >> 1;               /* floor((1+sqrt)/2) */

    /* Correct for possible rounding errors. */
    while ((__int128)(s - 1) * s / 2 < n) ++s;
    while (s > 1 && (__int128)(s - 2) * (s - 1) / 2 >= n) --s;

    idx_t prev = (__int128)(s - 2) * (s - 1) / 2;
    idx_t pos  = n - prev;          /* 1‑based position within diagonal s */

    /* Map to row (n) and column (k) indices (both 1‑based). */
    idx_t r = pos;
    idx_t k = s - pos;              /* k ≥ 1 */

    /* Compute T(r,k) = k^(r+1) + (k+1)^(r+1). */
    seq_t term1 = big_pow((seq_t)k,     (seq_t)(r + 1));
    seq_t term2 = big_pow((seq_t)(k + 1), (seq_t)(r + 1));

    return term1 + term2;
}
