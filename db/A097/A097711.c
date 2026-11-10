/*
 * A097711  Column 1 of triangle A097710, in which row (n) is formed from the
 *          sums of the adjacent terms in row (n-1) of the matrix square of
 *          A097710.
 * https://oeis.org/A097711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n+1,1), where T is A097710: T(0,0)=1, T(m,m)=1, T(m,0)
 *            = Sum_{j<m} T(m-1,j)*T(j,0), T(m,k) = Sum_{j<m}
 *            T(m-1,j)*(T(j,k-1)+T(j,k)) for 1<=k<m
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 68 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t R = (size_t)n + 2;
    seq_t *T = mem_new(seq_t, R * R);
    T[0] = (seq_t)1;
    for (size_t cur = 1; cur < R; ++cur) {
        T[cur * R + cur] = (seq_t)1;
        for (size_t k = 0; k < cur; ++k) {
            seq_t sum = 0;
            for (size_t j = 0; j < cur; ++j) {
                seq_t part = T[j * R + k];
                if (k > 0) part += T[j * R + k - 1];
                sum += T[(cur - 1) * R + j] * part;
            }
            T[cur * R + k] = sum;
        }
    }
    seq_t res = T[(size_t)(n + 1) * R + 1];
    mem_free(T);
    return res;
}
