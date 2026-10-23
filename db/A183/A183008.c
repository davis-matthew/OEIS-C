/*
 * A183008  a(n) = 24*p(n) = 24*A000041(n).
 * https://oeis.org/A183008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * p(n) where p(n) is the partition function
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of partition numbers */
    static seq_t *p   = NULL;   /* array of p(i) */
    static size_t  cap = 0;     /* currently allocated capacity */
    static idx_t   max = 0;     /* largest n computed so far */

    /* allocate the cache on first use */
    if (p == NULL) {
        cap = 1;
        p = (seq_t *)malloc(cap * sizeof(seq_t));
        if (p == NULL) return (seq_t)0;   /* allocation failed */
        p[0] = (seq_t)1;                   /* p(0) = 1 */
    }

    /* grow the cache if necessary, preserving old values */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 64;   /* give some slack */
        seq_t *q = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
        if (q == NULL) return (seq_t)0;   /* allocation failed */
        /* new slots are left uninitialized – they will be set when computed */
        p = q;
        cap = new_cap;
    }

    /* compute partitions up to n if we don't have them yet */
    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            /* Euler's pentagonal theorem:
               p(i) = Σ(-1)^(k-1)[ p(i - g_k) + p(i - g'_k) ]   */
            seq_t sum_odd  = 0;
            seq_t sum_even = 0;
            for (int k = 1; ; ++k) {
                /* generalized pentagonal numbers */
                idx_t g1 = (idx_t)((k * (3 * k - 1)) / 2);
                idx_t g2 = (idx_t)((k * (3 * k + 1)) / 2);
                if (g1 > i && g2 > i) break;   /* no more terms */

                if (g1 <= i) {
                    if (k & 1) sum_odd += p[i - g1];
                    else       sum_even += p[i - g1];
                }
                if (g2 <= i) {
                    if (k & 1) sum_odd += p[i - g2];
                    else       sum_even += p[i - g2];
                }
            }
            p[i] = sum_odd - sum_even; /* guaranteed non‑negative */
        }
        max = n;
    }

    /* return 24 * p(n) */
    return (seq_t)24 * p[n];
}
