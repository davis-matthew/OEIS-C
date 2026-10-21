/*
 * A209435  Table T(m,n), read by antidiagonals, is the number of subsets of
 *          {1,...,n} which do not contain two elements whose difference is
 *          m+1.
 * https://oeis.org/A209435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = product_{i=0..m} Fib( floor((k + i) / (m + 1)) + 2 ) *
 *            where (m,k) are obtained from n by anti‑diagonal reading of
 *            Table T(m,n): * find the unique s such that (s+1)(s+2)/2 > n, *
 *            let t = n - s(s+1)/2; * then m = s - t and k = t. * * The
 *            function computes a(n) for any n ≥ 0
 * Range:     u8 n <= 88, u16 n <= 298, u32 n <= 1126, u64 n <= 4369,
 *            u128 n <= 5049, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

static inline __int128 triangular(idx_t s)
{
    /* (s+1)(s+2)/2 computed in 128‑bit to avoid overflow */
    return (__int128)(s + 1) * (s + 2) / 2;
}

seq_t a(idx_t n)
{
    /*--- 1. Find s  ---*/
    idx_t low = 0, high = 1;
    while (triangular(high) <= n) {
        if (high == UINT64_MAX) break;
        high <<= 1;           /* multiply by 2 */
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;            /* first s with triangular(s) > n */

    /*--- 2. Compute (m,k) ---*/
    idx_t prev_diag = (s == 0) ? 0 : (idx_t)(triangular(s - 1));
    idx_t t       = n - prev_diag;          /* position in anti‑diagonal (0‑based) */
    idx_t m       = s - t;
    idx_t k       = t;                      /* column index */

    /*--- 3. Product of Fibonacci terms ---*/
    seq_t result = 1;
    if (m > 0)
        result = 1;   /* ensure initialization for m==0 case below */

    for (idx_t i = 0; i <= m; ++i) {
        idx_t val = (k + i) / (m + 1) + 2;
        result *= fib(val);
    }

    return result;
}
