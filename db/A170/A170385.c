/*
 * A170385  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence from the rational generating function: a(0) = 1 a(n)
 *            = 2 + 38 * sum_{i=0}^{n-1} a[i] for 1 ≤ n ≤ 42 a(n) = 38 *
 *            sum_{i=1}^{42} a[n-i] - 741 * a[n-43] for n ≥ 43
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    seq_t *a = alloc_seq_array(len);

    a[0] = (seq_t)1;
    if (n == 0) {
        seq_t res = a[0];
        free_seq_array(a);
        return res;
    }

    /* compute terms up to n = 42 (or less if n < 42) */
    seq_t sumPrev = a[0];
    idx_t i;
    idx_t limit = n < 42 ? n : 42;
    for (i = 1; i <= limit; ++i) {
        seq_t term = mul_small(sumPrev, 38);
        seq_t pi = (seq_t)2;
        a[i] = pi + term;
        sumPrev += a[i];
    }

    if (n <= 42) {
        seq_t res = a[n];
        free_seq_array(a);
        return res;
    }

    /* compute term n ≥ 43 using the linear recurrence */
    seq_t window_sum = (seq_t)0;
    for (idx_t j = 1; j <= 42; ++j) {
        window_sum += a[j];
    }

    for (i = 43; i <= n; ++i) {
        seq_t t1 = mul_small(window_sum, 38);
        seq_t t2 = mul_small(a[i - 43], 741);
        a[i] = t1 - t2;
        window_sum += a[i];
        window_sum -= a[i - 42];
    }

    seq_t res = a[n];
    free_seq_array(a);
    return res;
}
