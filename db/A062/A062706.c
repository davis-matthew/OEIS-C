/*
 * A062706  The array of A062704 read by diagonals in the 'up' direction.
 * https://oeis.org/A062706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   A062704 array T(r,c) read by antidiagonals "up": a(n) = T(d-c,
 *            c), where d(d+1)/2 < n <= (d+1)(d+2)/2, c = n-1-d(d+1)/2; T is
 *            filled diagonal by diagonal as in A062704 (diagonals alternate
 *            up/down, each starts with 1, entry = previous on diagonal + up
 *            to two previous in row/column)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t dd = 0;
    while ((dd + 1) * (dd + 2) / 2 < n) ++dd;
    size_t c0 = (size_t)(n - 1 - dd * (dd + 1) / 2);
    const size_t d_max = (size_t)dd;
    const size_t D = d_max + 1;
    seq_t *T = mem_new(seq_t, D * D);
    if (!T) abort();
    T[0] = (seq_t)1;
    for (size_t d = 1; d <= d_max; ++d) {
        if (d & 1) {
            T[d * D] = (seq_t)1;
            for (size_t c = 1; c <= d; ++c) {
                size_t r = d - c;
                seq_t v = T[(r + 1) * D + c - 1];
                v += T[r * D + c - 1];
                if (c >= 2) v += T[r * D + c - 2];
                T[r * D + c] = v;
            }
        } else {
            T[d] = (seq_t)1;
            for (size_t r = 1; r <= d; ++r) {
                size_t c = d - r;
                seq_t v = T[(r - 1) * D + c + 1];
                v += T[(r - 1) * D + c];
                if (r >= 2) v += T[(r - 2) * D + c];
                T[r * D + c] = v;
            }
        }
    }
    seq_t res = T[(d_max - c0) * D + c0];
    mem_free(T);
    return res;
}
