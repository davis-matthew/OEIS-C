/*
 * A052343  Number of ways to write n as the unordered sum of two triangular
 *          numbers (zero allowed).
 * https://oeis.org/A052343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of triangular numbers
 *            (including 0) summing to n
 * Range:     u8 n <= 504, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over all triangular numbers ≤ n and checks whether the complement
   n - T_i is also triangular.  A number m is triangular iff 8*m + 1 is a perfect square
   whose square root is odd.  The integer square root for 64‑bit values is computed
   with a binary search. */

#include <stddef.h>

/* Integer square root for a 64‑bit value (floor). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(UINT64_MAX)               */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t t   = 0;                         /* current triangular number T_i          */
    seq_t n_seq = (seq_t)n;                /* same value in seq_t type               */

    for (idx_t i = 0; t <= n_seq; ++i) {
        seq_t rem = n_seq - t;             /* n - T_i                                */
        if (rem >= t) {                    /* enforce unordered (i ≤ j)              */
            idx_t u = (idx_t)rem * (idx_t)8 + 1;   /* 8*rem + 1, safe because rem ≤ n     */
            idx_t s = isqrt_u64(u);
            if ((idx_t)s * (idx_t)s == u && (s & 1))
                ans++;
        }
        t += (seq_t)(i + 1);               /* T_{i+1} = T_i + (i+1)                 */
    }
    return ans;
}
