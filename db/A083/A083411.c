/*
 * A083411  a(n) = Sum(((k-1)/2)*k!*Stirling_2(n,k),k=1..n).
 * https://oeis.org/A083411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(((k-1)/2)*k!*Stirling_2(n,k),k=1..n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum_{k=2..n} (k-1) * (k!/2) * S2(n,k). The row S2(n,.) comes from
   S2(m,k) = k S2(m-1,k) + S2(m-1,k-1), and k!/2 = 3*4*...*k is a plain product,
   so every step is a ring operation and the result is exact modulo the width
   (the original divided (k-1)*k! by 2 after it had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t *S = mem_new(seq_t, (size_t)n + 1);     /* S[k] = S2(m,k), zero */
    if (!S) abort();
    S[0] = (seq_t)1;                               /* S2(0,0) */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k > 0; --k) S[k] = (seq_t)k * S[k] + S[k - 1];
        S[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0, half_fact = (seq_t)1;    /* k!/2, from k = 2 */
    for (idx_t k = 2; k <= n; ++k) {
        if (k > 2) half_fact *= (seq_t)k;
        sum += (seq_t)(k - 1) * half_fact * S[k];
    }
    mem_free(S);
    return sum;
}
