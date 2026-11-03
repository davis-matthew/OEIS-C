/*
 * A057041  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057041(n)=j(F(n)), where F(n) is
 *          the n-th Fibonacci number.
 * https://oeis.org/A057041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(F(n)) where j(k) is the column index of k in the
 *            anti-diagonal enumeration of the infinite rectangular array
 *            R(i,j)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   fib_pair, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the defined offset). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Compute the n-th Fibonacci number F(n). */
    seq_t fn, fn_next;
    fib_pair(n, &fn, &fn_next);      /* fn = F(n), fn_next = F(n+1) */
    seq_t k = fn;                     /* Position number in the enumeration */

    /* Find diagonal d such that d(d-1)/2 < k ≤ d(d+1)/2.
       Solve  d(d+1)/2 ≥ k  →  d ≥ ceil((sqrt(8k+1)-1)/2). */
    seq_t t = k * (seq_t)8 + (seq_t)1;          /* 8k + 1  */
    seq_t s = floor_sqrt(t);                   /* floor(sqrt(8k+1)) */
    seq_t d;
    if (s * s == t) {
        /* sqrt(8k+1) is integer: use floor directly */
        d = (s - (seq_t)1) / (seq_t)2;
    } else {
        /* ceil((sqrt-1)/2) */
        d = (s + (seq_t)1) / (seq_t)2;
    }

    /* Compute the position within the d-th diagonal: */
    seq_t prev_sum = (d - (seq_t)1) * d / (seq_t)2;   /* sum of first d-1 diagonals */
    seq_t pos = k - prev_sum;                         /* 1 ≤ pos ≤ d */
    seq_t j = d - pos + (seq_t)1;                      /* column index */

    return j;
}
