/*
 * A029863  Expansion of Product_{k >= 1} 1/(1-x^k)^c(k), where c(1), c(2),
 *          ... = 2 3 2 3 2 3 2 3 ....
 * https://oeis.org/A029863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of partitions of n with 2 kinds of odd parts and
 * 3 kinds of even parts.  Computed via the Euler transform of the sequence
 * c(k) where c(k)=2 for odd k and 3 for even k.  The recurrence used is:
 *
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1}^n B(k) * a(n-k),   n ≥ 1
 *
 *   where B(k) = Σ_{d|k} d * c(d)
 *
 * The array of results is cached and extended on demand, so later calls
 * with larger n are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache variables ------------------------------------------------- */
    static seq_t *a_cache = NULL;   /* aCache[0..max_n] */
    static seq_t *B        = NULL;   /* B[0..max_n]   (B[0] unused) */
    static idx_t  max_n   = 0;        /* highest n computed so far */

    /* initialize cache if this is the first call ---------------------------- */
    if (max_n == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        B       = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;  /* a(0) = 1 */
        B[0]       = (seq_t)0;
        max_n      = 0;
    }

    /* if n is zero we already have the answer ------------------------------ */
    if (n == 0)
        return a_cache[0];

    /* extend cache to reach n ------------------------------------------------ */
    if (n > max_n) {
        idx_t new_n = n;

        /* reallocate arrays for the new maximum */
        a_cache = (seq_t *)realloc(a_cache, (new_n + 1) * sizeof(seq_t));
        B       = (seq_t *)realloc(B,       (new_n + 1) * sizeof(seq_t));

        /* zero the B array for the new range */
        for (idx_t i = 0; i <= new_n; ++i)
            B[i] = (seq_t)0;

        /* compute B[k] = Σ_{d|k} d * c(d)   (c(d)=2 if d odd else 3) */
        for (idx_t d = 1; d <= new_n; ++d) {
            seq_t ck   = (seq_t)((d & 1u) ? 2 : 3);      /* c(d) */
            seq_t term = ck * (seq_t)d;                 /* d * c(d) */
            for (idx_t m = d; m <= new_n; m += d)
                B[m] += term;
        }

        /* compute a[i] for i = max_n+1 .. new_n ---------------------------- */
        /* ensure aCache[0] is set (may not have been set before)           */
        if (max_n == 0)
            a_cache[0] = (seq_t)1;

        for (idx_t i = max_n + 1; i <= new_n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k)
                sum += B[k] * a_cache[i - k];
            a_cache[i] = sum / (seq_t)i;                 /* exact division */
        }

        max_n = new_n;
    }

    return a_cache[n];
}
