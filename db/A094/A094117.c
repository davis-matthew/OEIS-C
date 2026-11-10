/*
 * A094117  Partial sums of A093349.
 * https://oeis.org/A094117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} b(k), b(1)=0, b(k) = Sum_{i=1..k-1}
 *            (-1)^(i-1)*7^v7(i) (b = A093349)
 * Range:     u8 n <= 51, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A094117_cache = NULL;
static idx_t A094117_count = 0;
static seq_t A094117_b = 0;   /* A093349(A094117_count) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > A094117_count) {
        seq_t *nc = mem_resize(seq_t, A094117_cache, n);
        if (!nc) return (seq_t)0;
        A094117_cache = nc;
        for (idx_t k = A094117_count + 1; k <= n; ++k) {
            seq_t prev = (k == 1) ? (seq_t)0 : A094117_cache[k - 2];
            if (k == 1) {
                A094117_b = 0;
            } else {
                idx_t i = k - 1;
                seq_t term = 1;
                while (i % 7 == 0) { i /= 7; term *= 7; }
                if ((k - 1) & 1) A094117_b += term;
                else A094117_b -= term;
            }
            A094117_cache[k - 1] = prev + A094117_b;
        }
        A094117_count = n;
    }
    return A094117_cache[n - 1];
}
