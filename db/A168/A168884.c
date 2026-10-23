/*
 * A168884  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1: a(n) = 13 * Σ_{k=1}^{20} a(n‑k) – 91 *
 *            a(n‑21) + b_n where b_n = 1 if n = 0 or n = 21 b_n = 2 if 1 ≤ n
 *            ≤ 20 b_n = 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function returns a(n) for any n ≥ 0.  The values are
   cached so that subsequent calls are answered in O(1) amortised time.
   All arithmetic is performed in seq_t, which may be an unsigned
   integer of any width or an arbitrary–precision integer.  */

/* Include needed headers */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache that grows as needed.  */
    static seq_t *cache = NULL;
    static idx_t  size   = 0;   /* number of computed entries */
    static size_t cap    = 0;   /* allocated capacity */

    if (cache == NULL) {            /* first call: initialise */
        cap = 64;                   /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – caller will handle */
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        size = 1;
    }

    /* Grow the cache if n is beyond current capacity.  */
    if ((idx_t)(n + 1) > (idx_t)cap) {
        size_t new_cap = cap * 2;
        while ((idx_t)(n + 1) > (idx_t)new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing entries up to n.  */
    while (size <= n) {
        idx_t i = size;          /* index to compute */
        seq_t sum = (seq_t)0;

        /* Σ_{k=1}^{20} a(i‑k) * 13  */
        for (int k = 1; k <= 20; ++k) {
            if (i >= (idx_t)k)
                sum += (seq_t)13 * cache[i - k];
        }

        /* 91 * a(i‑21)  */
        seq_t a21 = (i >= 21) ? cache[i - 21] : (seq_t)0;

        /* b_i   */
        seq_t b;
        if (i == 0 || i == 21)
            b = (seq_t)1;
        else if (i >= 1 && i <= 20)
            b = (seq_t)2;
        else
            b = (seq_t)0;

        cache[i] = sum - (seq_t)91 * a21 + b;
        ++size;
    }

    return cache[n];
}
