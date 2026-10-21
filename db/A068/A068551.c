/*
 * A068551  a(n) = 4^n - binomial(2*n,n).
 * https://oeis.org/A068551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4^n - binomial(2*n, n) Recurrence: a(0) = 0, a(1) = 2,
 *            a(n+1) = 4*a(n) + 2*C(n) (C(n) = Catalan number) with C(0)=1 and
 *            C(k+1) = C(k) * (4*k-2) / (k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   _ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_ensure_capacity.h"

/* External linkage sequence function */
seq_t a(idx_t n)
{
    /* static caches for computed a(n) and Catalan numbers */
    static seq_t *a_cache = NULL;     /* array of a values */
    static size_t a_cap = 0, a_len = 0;/* current capacity and length of a_cache */
    static seq_t *cat_cache = NULL;   /* array of Catalan numbers C(n) */
    static size_t cat_cap = 0, cat_len = 0;/* capacity and length of cat_cache */

    /* initialise caches for n = 0 */
    if (a_len == 0) {
        _ensure_capacity(1, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[0] = (seq_t)0;
        a_len = 1;
    }
    /* initialise Catalan cache for C(0) = 1 */
    if (cat_len == 0) {
        _ensure_capacity(1, &cat_cap, (void **)&cat_cache, sizeof(seq_t));
        cat_cache[0] = (seq_t)1;
        cat_len = 1;
    }

    if (n < a_len)
        return a_cache[n];

    /* Ensure arrays large enough for indices 0..n */
    _ensure_capacity(n + 1, &a_cap, (void **)&a_cache, sizeof(seq_t));
    _ensure_capacity(n + 1, &cat_cap, (void **)&cat_cache, sizeof(seq_t));

    /* Compute terms from current length up to n, inclusive */
    for (idx_t i = a_len; i <= n; ++i) {
        /* a(i) uses Catalan C(i-1) which is already computed for i>=1 */
        seq_t prev_a  = a_cache[i - 1];
        seq_t cat_prev = cat_cache[i - 1];
        seq_t a_next = prev_a * (seq_t)4 + cat_prev * (seq_t)2;
        a_cache[i] = a_next;

        /* Compute Catalan C(i) using recurrence: C(i) = C(i-1) * (4*i-2)/(i+1) */
        seq_t factor = (seq_t)(4 * i - 2);
        seq_t tmp = cat_prev * factor;
        seq_t cat_next = tmp / (seq_t)(i + 1);
        cat_cache[i] = cat_next;
    }

    a_len = n + 1;
    cat_len = n + 1;
    return a_cache[n];
}
