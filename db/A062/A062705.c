/*
 * A062705  Array described in A062704 read by diagonals in direction of
 *          creation.
 * https://oeis.org/A062705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   triangle read by rows; row d (d>=0) lists the diagonal r+c=d of
 *            the A062704 array in creation order (odd d: from (d,0) up to
 *            (0,d); even d: from (0,d) down to (d,0)); each entry = previous
 *            diagonal entry + the two preceding entries in its row (up) or
 *            column (down); each diagonal starts with 1; a(...
 * Range:     u8 n <= 23, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
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
    size_t d = 0;
    idx_t rem = n;
    while (rem > (idx_t)(d + 1)) {
        rem -= (idx_t)(d + 1);
        ++d;
    }
    size_t k = (size_t)(rem - 1);
    const size_t D = d + 1;
    seq_t *T = mem_new(seq_t, D * D);
    if (!T) abort();
    T[0] = (seq_t)1;
    for (size_t e = 1; e <= d; ++e) {
        if (e & 1) {
            T[e * D] = (seq_t)1;
            for (size_t c = 1; c <= e; ++c) {
                size_t r = e - c;
                seq_t v = T[(r + 1) * D + c - 1];
                v += T[r * D + c - 1];
                if (c >= 2) v += T[r * D + c - 2];
                T[r * D + c] = v;
            }
        } else {
            T[e] = (seq_t)1;
            for (size_t r = 1; r <= e; ++r) {
                size_t c = e - r;
                seq_t v = T[(r - 1) * D + c + 1];
                v += T[(r - 1) * D + c];
                if (r >= 2) v += T[(r - 2) * D + c];
                T[r * D + c] = v;
            }
        }
    }
    seq_t res;
    if (d & 1) res = T[(d - k) * D + k];
    else res = T[k * D + (d - k)];
    mem_free(T);
    return res;
}
