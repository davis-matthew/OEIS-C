/*
 * A051474  (Terms in A014450)/2.
 * https://oeis.org/A051474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014450(n+1)/2, where A014450 lists the even Eulerian
 *            numbers of the triangle T(m,k) = (k+1)*T(m-1,k) +
 *            (m-k)*T(m-1,k-1), T(1,0) = 1, read by rows; computed by scanning
 *            rows
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t target = n + 1;
    idx_t even_cnt = 0;
    seq_t *p = mem_new(seq_t, 1);
    p[0] = 1;
    for (idx_t row = 1; ; ++row) {
        if (row > 1) {
            p = mem_resize(seq_t, p, row);
            p[row - 1] = 0;
            for (idx_t k = row; k-- > 0; ) {
                seq_t v = (seq_t)(k + 1) * p[k];
                if (k > 0) v += (seq_t)(row - k) * p[k - 1];
                p[k] = v;
            }
        }
        for (idx_t k = 0; k < row; ++k) {
            if ((p[k] & 1) == 0) {
                ++even_cnt;
                if (even_cnt == target) {
                    seq_t r = p[k] / 2;
                    mem_free(p);
                    return r;
                }
            }
        }
    }
}
