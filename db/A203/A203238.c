/*
 * A203238  a(n)=(n-1)-st elementary symmetric function of the first n terms
 *          of (2, -4, 6, -8, 10, ...).
 * https://oeis.org/A203238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{i=1}^n ∏_{j≠i} (2j·(-1)^{j+1}) (the (n‑1)-st
 *            elementary symmetric function of the first n terms of (2, -4, 6,
 *            -8, 10, …))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1,  a(n)=x_n·a(n‑1)+P_{n‑1}  with  x_n=(-1)^{n+1}·2n and P_k=∏_{i=1}^k x_i. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *p_cache = NULL;
    static idx_t max_n = 0;
    static size_t cap = 0;

    if (cap == 0) {
        cap = 16;
        a_cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        p_cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!a_cache || !p_cache) return 0;

        p_cache[0] = 1;
        a_cache[0] = 0;

        a_cache[1] = 1;
        p_cache[1] = 2;
        max_n = 1;
    }

    if (n > max_n) {
        if (n > cap) {
            size_t newcap = cap ? cap << 1 : 16;
            while (newcap < (size_t)n) newcap <<= 1;
            seq_t *tmpa = (seq_t*)realloc(a_cache, (newcap + 1) * sizeof(seq_t));
            seq_t *tmpb = (seq_t*)realloc(p_cache, (newcap + 1) * sizeof(seq_t));
            if (!tmpa || !tmpb) return 0;
            a_cache = tmpa;
            p_cache = tmpb;
            cap = newcap;
        }
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t xi = (seq_t)i * 2ULL;
            if ((i & 1) == 0) xi = -xi;
            p_cache[i] = p_cache[i - 1] * xi;
            a_cache[i] = xi * a_cache[i - 1] + p_cache[i - 1];
        }
        max_n = n;
    }

    return a_cache[n];
}
