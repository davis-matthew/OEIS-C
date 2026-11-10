/*
 * A087160  Satisfies a(1)=1, a(2)=2, a(n+2)=a(n+1)+A087159(n)+1.
 * https://oeis.org/A087160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n+2)=a(n+1)+A087159(n)+1, where A087159(1)=1,
 *            A087159(m)=2 for m not in this sequence, and A087159(a(n+1)) =
 *            A087159(n)+1
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t *A = mem_new(seq_t, (size_t)(n + 1));
    if (!A) return 0;
    for (idx_t i = 1; i <= n; ++i) A[i] = 2;
    A[1] = 1;

    seq_t b2 = 2;
    for (idx_t k = 1; k + 2 <= n; ++k) {
        seq_t next = b2 + A[k] + 1;
        if (next <= (seq_t)n && next > b2) {
            A[(size_t)next] = A[k + 1] + 1;
        }
        b2 = next;
    }

    mem_free(A);
    return b2;
}
