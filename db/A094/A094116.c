/*
 * A094116  Partial sums of A093348.
 * https://oeis.org/A094116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A093348(k); A093348(1)=0,
 *            A093348(m)=w-A093348(m-w) for m>=2, w=largest power of 5 <= m-1
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A094116_base = NULL;
static seq_t *A094116_sum = NULL;
static idx_t A094116_sz = 0;

static idx_t A094116_w(idx_t m)
{
    idx_t w = 1;
    while (w <= (m - 1) / 5) {
        w *= 5;
    }
    return w;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if (n > A094116_sz) {
        seq_t *b = mem_resize(seq_t, A094116_base, n + 1);
        if (!b) return (seq_t)0;
        A094116_base = b;
        seq_t *s = mem_resize(seq_t, A094116_sum, n + 1);
        if (!s) return (seq_t)0;
        A094116_sum = s;
        for (idx_t i = A094116_sz + 1; i <= n; ++i) {
            if (i == 1) {
                A094116_base[i] = (seq_t)0;
                A094116_sum[i] = (seq_t)0;
            } else {
                idx_t w = A094116_w(i);
                A094116_base[i] = (seq_t)w - A094116_base[i - w];
                A094116_sum[i] = A094116_sum[i - 1] + A094116_base[i];
            }
        }
        A094116_sz = n;
    }
    return A094116_sum[n];
}
