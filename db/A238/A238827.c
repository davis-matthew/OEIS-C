/*
 * A238827  a(n) = 0 for n <= 3; thereafter a(n) = a(n-2)+A238825(n-3).
 * https://oeis.org/A238827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + A238825(n-3) for n > 3, a(n) = 0 for n <= 3;
 *            A238825: a(1..12)=0,0,0,1,2,5,11,27,64,158,387,956 and for n>12
 *            a(n) = 3a(n-1) - 4a(n-3) + a(n-4) + a(n-5) + 3a(n-6) - a(n-7)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 31, u64 n <= 56, u128 n <= 105,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 912 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A238825_val(const seq_t *c, idx_t i)
{
    return c[i];
}

seq_t a(idx_t n)
{
    if (n <= 3) return 0;
    idx_t m = n - 3;
    idx_t sz = (m < 12 ? 12 : m) + 1;
    static const seq_t base[13] = {0, 0, 0, 0, 1, 2, 5, 11, 27, 64, 158, 387, 956};
    seq_t *A = mem_new(seq_t, (size_t)sz);
    if (!A) abort();
    for (idx_t i = 1; i <= (idx_t)(sz - 1); ++i) {
        if (i <= 12) {
            A[i] = base[i];
        } else {
            seq_t pos = A[i-1] * 3 + A[i-4] + A[i-5] + A[i-6] * 3;
            seq_t neg = A[i-3] * 4 + A[i-7];
            A[i] = pos - neg;
        }
    }
    seq_t s = 0;
    idx_t j = m;
    for (;;) {
        s += A238825_val(A, j);
        if (j <= 2) break;
        j -= 2;
    }
    mem_free(A);
    return s;
}
