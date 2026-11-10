/*
 * A054148  T(2n+1,n), array T as in A054144.
 * https://oeis.org/A054148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(2n+1,n), where T(r,0) = r and, for k >= 1, * T(r,k) =
 *            Sum_{i=r-k..r} Sum_{j=0..i-r+k} T(i,j) (T(r,k) itself taken as
 *            0). * With m = r-k and P(i,c) = Sum_{j=0..c} T(i,j): * T(r,k) =
 *            A_m + P(r,k-1), where A_m = Sum_{i=m..r-1} P(i,i-m), * and A_m
 *            is accumulated row by row
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t R = 2 * N + 1;
    seq_t *acc = mem_new(seq_t, R + 2);   /* acc[m] = Sum P(i, i-m) over rows done */
    seq_t result = 0;

    for (size_t r = 0; r <= R; ++r) {
        seq_t pref = 0;                   /* P(r, k-1) */
        size_t kmax = (r < N) ? r : N;
        for (size_t k = 0; k <= kmax; ++k) {
            size_t m = r - k;
            seq_t t;
            if (k == 0) t = (seq_t)r;
            else        t = acc[m] + pref;
            pref += t;                    /* now P(r, k) */
            acc[m] += pref;               /* add P(r, r-m) */
            if (r == R && k == N) result = t;
        }
    }

    mem_free(acc);
    return result;
}
