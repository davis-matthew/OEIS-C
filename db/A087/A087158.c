/*
 * A087158  Satisfies a(n+2)=a(n+1)+A087157(n)+1.
 * https://oeis.org/A087158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+2) = a(n+1) + A087157(n) + 1, a(1)=1, a(2)=2; with
 *            A087157(1)=1, A087157(a(n+1)) = A087157(n)+1, and A087157(m)=1
 *            for m not in the sequence
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t *A = mem_new(seq_t, n + 2);
    if (!A) abort();
    for (idx_t i = 0; i <= n + 1; ++i) A[i] = (seq_t)1;
    seq_t b = (seq_t)2;          /* A087158(j+1) */
    seq_t lim = (seq_t)n;
    for (idx_t j = 1; j + 2 <= n; ++j) {
        if (b <= lim) A[(long long)b] = A[j] + (seq_t)1;
        b = b + A[j] + (seq_t)1;  /* A087158(j+2) */
    }
    mem_free(A);
    return b;
}
