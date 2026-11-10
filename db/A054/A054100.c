/*
 * A054100  T(n,n), array T as in A054098.
 * https://oeis.org/A054100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) of A054098: T(n,0)=1, T(1,1)=2, T(n,1)=r(n-1),
 *            T(n,k)=T(n,k-1)+(-1)^(k+1)*r(n-k) for 2<=k<=n, r(h)=sum of row
 *            h; a(0)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *rs  = mem_new(seq_t, N + 2);
    seq_t *tmp = mem_new(seq_t, N + 2);
    seq_t result = 1;
    for (size_t cur = 0; cur <= N; ++cur) {
        tmp[0] = 1;
        if (cur == 1) {
            tmp[1] = 2;
        } else if (cur >= 2) {
            tmp[1] = rs[cur - 1];
            for (size_t k = 2; k <= cur; ++k) {
                if (k & 1) tmp[k] = tmp[k - 1] + rs[cur - k];
                else       tmp[k] = tmp[k - 1] - rs[cur - k];
            }
        }
        seq_t s = 0;
        for (size_t k = 0; k <= cur; ++k) s += tmp[k];
        rs[cur] = s;
        result = tmp[cur];
    }
    mem_free(rs);
    mem_free(tmp);
    return result;
}
