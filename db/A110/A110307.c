/*
 * A110307  Expansion of (1+2*x)/((1+x+x^2)*(1+5*x+x^2)).
 * https://oeis.org/A110307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4); Initial values:
 *            a(0)=1, a(1)=-4, a(2)=17, a(3)=-80. This is the expansion of
 *            (1+2x)/((1+x+x^2)*(1+5x+x^2))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 28, i128 n <= 56,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t capacity = 0;
    static const seq_t init[4] = { (seq_t)1, (seq_t)-4, (seq_t)17, (seq_t)-80 };

    if (size == 0) {
        capacity = 4;
        size = 4;
        seq = seq_malloc(capacity);
        for (idx_t i = 0; i < 4; ++i) seq[i] = init[i];
    }

    if (n + 1 > capacity) {
        capacity = n + 1;
        seq = seq_realloc(seq, capacity);
    }

    if (n + 1 > size) {
        for (idx_t i = size; i <= n; ++i) {
            seq[i] = -(seq_t)6 * seq[i-1]
                     - (seq_t)7 * seq[i-2]
                     - (seq_t)6 * seq[i-3]
                     - seq[i-4];
        }
        size = n + 1;
    }

    return seq[n];
}
