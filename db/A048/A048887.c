/*
 * A048887  Array T read by antidiagonals, where T(m,n) = number of
 *          compositions of n into parts <= m.
 * https://oeis.org/A048887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 227, u32 n <= 736, u64 n <= 2550,
 *            u128 n <= 9309, big any n
 * Helpers:   first_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A048887:
   a(n) is obtained by reading an infinite array T(m,n) along
   antidiagonals, where T(m,n) is the number of compositions of
   n into parts not exceeding m.  The algorithm maps the index n
   to the corresponding pair (m,n) and evaluates T(m,n) by
   dynamic programming using a sliding‑window sum. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/first_t.h"

seq_t a(idx_t n)
{
    /* Find the diagonal number t (t ≥ 1) containing position n. */
    idx_t t = first_t(n);

    /* Total elements before this diagonal. */
    __uint128_t prev128 = (__uint128_t)(t - 1) * t / 2;
    idx_t prev = (idx_t)prev128;

    /* Zero‑based position inside the diagonal. */
    idx_t k = n - prev - 1;

    /* Coordinates of the T(m,npos) cell. */
    idx_t m      = t - k;          /* 1 ≤ m ≤ t            */
    idx_t npos   = (t + 1) - m;    /* 1 ≤ npos ≤ t         */

    /* Allocate an array for dynamic programming.  Use new[] in C++ and
       calloc in plain C so that construction works for arbitrary‑precision
       integer classes. */
#if defined(__cplusplus)
    seq_t *dp = new seq_t[(size_t)(npos + 1)];
#else
    seq_t *dp = (seq_t*)calloc((size_t)(npos + 1), sizeof(seq_t));
#endif
    if (!dp) return 0;    /* allocation failed */

    dp[0] = 1;
    seq_t window = 1;      /* sum of the last m values of dp */

    for (idx_t i = 1; i <= npos; ++i) {
        dp[i] = window;              /* current value */

        window += dp[i];             /* update window for next i */
        if (i >= m)
            window -= dp[i - m];
    }

    seq_t result = dp[npos];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
