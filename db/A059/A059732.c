/*
 * A059732  Central column (even terms a(2*n,n)) of A059922.
 * https://oeis.org/A059732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(2n,n) where T(m,k) = T(m-1,k-1)*T(m-1,k)+1, T(0,0)=1,
 *            T(m,0)=T(m,m)=1 (A059922 triangle)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-07)
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
    size_t R = 2 * N;
    seq_t *row = mem_new(seq_t, N + 2);
    row[0] = 1;
    for (size_t r = 1; r <= R; ++r)
    {
        size_t top = r - 1 < N ? r - 1 : N;
        if (r <= N) row[r] = 1;
        for (size_t j = top; j >= 1; --j)
        {
            row[j] = row[j - 1] * row[j] + 1;
        }
    }
    seq_t res = row[N];
    mem_free(row);
    return res;
}
