/*
 * A066668  Signed row sums of A066667.
 * https://oeis.org/A066668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = n! * LaguerreL(n, 1, 1) Recurrence: a(0)=1, a(1)=1,
 *            a(2)=1 a(n)+(-2*n+1)*a(n-1)+n*(n-1)*a(n-2)=0 (n≥1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A066668 (Signed row sums of A066667). */
seq_t a(idx_t n)
{
    /* Static cache: store a(0)..a(k) for the largest already computed k. */
    static seq_t *cache = NULL;      /* dynamic array of terms */
    static idx_t cache_cap = 0;      /* number of valid entries (0..cache_cap-1) */

    /* Increase capacity if needed. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) {   /* memory allocation failed – abort */
            exit(1);
        }
        cache = tmp;
        /* Fill new entries up to new_cap-1. */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                cache[i] = (seq_t)1;
            } else {
                /* Recurrence: a(i) = - [(-2*i+1)*a(i-1) + i*(i-1)*a(i-2)] */
                seq_t term = ((seq_t)((long long)(-2LL * (long long)i + 1LL))) *
                              cache[i - 1]
                           + (seq_t)(i) * (seq_t)(i - 1) * cache[i - 2];
                cache[i] = -term;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
