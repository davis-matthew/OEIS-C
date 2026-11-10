/*
 * A089836  INVERT transform of A089835.
 * https://oeis.org/A089836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A089835(k)*a(n-k), a(0)=1, where A089835(k)
 *            = Catalan(k)^2*(k+1)! (INVERT transform)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 126 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A089835_a(idx_t n)
{
    seq_t cat = (seq_t)1;
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        cat = cat * (seq_t)(4 * i - 2);
        cat = cat / (seq_t)(i + 1);
        fact = fact * (seq_t)(i + 1);
    }
    return cat * cat * fact;
}

seq_t a(idx_t n)
{
    seq_t *c = mem_new(seq_t, (size_t)n + 1);
    seq_t *b = mem_new(seq_t, (size_t)n + 1);
    /* compute c[k] = A089835(k) incrementally */
    seq_t cat = (seq_t)1;
    seq_t fact = (seq_t)1;
    c[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        cat = cat * (seq_t)(4 * i - 2);
        cat = cat / (seq_t)(i + 1);
        fact = fact * (seq_t)(i + 1);
        c[i] = cat * cat * fact;
    }
    b[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t s = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
            s += c[k] * b[m - k];
        b[m] = s;
    }
    seq_t r = b[n];
    mem_free(c);
    mem_free(b);
    return r;
}
