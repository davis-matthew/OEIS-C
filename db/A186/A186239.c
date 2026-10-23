/*
 * A186239  G.f. A(x) satisfies A(x) = 1+x*A(x)+x^2*A(x)^2+2*x^3*A(x)^3.
 * https://oeis.org/A186239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186239
 * Recurrence (derived from A(x) = 1 + x*A + x^2*A^2 + 2*x^3*A^3):
 *   a(0) = 1
 *   a(n) = a(n-1)
 *          + c2[n-2]                               if n ≥ 2
 *          + 2 * Σ_{i=0}^{n-3} a[i] * c2[n-3-i]     if n ≥ 3
 * where c2[k] = Σ_{i=0}^{k} a[i] * a[k-i]   (k ≥ 0)
 * The code memoises a(n) and the convolution array c2.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static idx_t  cur_max = (idx_t)-1;      /* highest n already computed   */
    static seq_t *a_cache = NULL;          /* a_cache[0..cur_max]          */
    static seq_t *c2_cache = NULL;         /* c2_cache[0..2*cur_max]      */
    static size_t c2_alloc = 0;            /* allocated size of c2_cache   */

    /* ------------------------------------------------------------------ */
    /* First time: allocate cache for n = 0                                 */
    if (cur_max == (idx_t)-1) {
        a_cache  = (seq_t *)calloc(1, sizeof(seq_t));
        c2_cache = (seq_t *)calloc(1, sizeof(seq_t));
        a_cache[0]  = 1;          /* a(0) = 1                         */
        c2_cache[0] = 1;          /* c2[0] = a0*a0 = 1                */
        c2_alloc = 1;
        cur_max = 0;
    }

    /* ------------------------------------------------------------------ */
    /* Expand memoised tables up to the requested index n                  */
    if (n > cur_max) {
        idx_t new_max = n;

        /* Grow a_cache */
        a_cache = (seq_t *)realloc(a_cache, (size_t)(new_max + 1) * sizeof(seq_t));

        /* Grow c2_cache */
        size_t new_c2_alloc = 2 * new_max + 1;
        if (new_c2_alloc > c2_alloc) {
            c2_cache = (seq_t *)realloc(c2_cache,
                                        new_c2_alloc * sizeof(seq_t));
            /* Zero any newly allocated portion */
            size_t extra = new_c2_alloc - c2_alloc;
            seq_t *ptr = c2_cache + c2_alloc;
            for (size_t i = 0; i < extra; ++i) ptr[i] = 0;
            c2_alloc = new_c2_alloc;
        }

        /* Compute missing terms */
        for (idx_t k = cur_max + 1; k <= new_max; ++k) {
            seq_t ak = a_cache[k - 1];          /* a(k-1) */

            if (k >= 2) ak += c2_cache[k - 2];  /* + c2[k-2] */

            if (k >= 3) {
                idx_t lim  = k - 3;
                seq_t sum3 = 0;
                for (idx_t i = 0; i <= lim; ++i)
                    sum3 += a_cache[i] * c2_cache[lim - i];
                ak += sum3 + sum3;              /* 2 * triple sum */
            }

            a_cache[k] = ak;

            /* Update convolution array c2 with new term a(k)            */
            /* For i < k : add 2*a[k]*a[i] to c2[k+i]                    */
            for (idx_t i = 0; i < k; ++i) {
                size_t idx = (size_t)(k + i);
                c2_cache[idx] += a_cache[k] * a_cache[i] * 2;
            }
            /* diagonal term a[k]*a[k] at 2*k */
            size_t diag = (size_t)(2 * k);
            c2_cache[diag] += a_cache[k] * a_cache[k];
        }
        cur_max = new_max;
    }

    /* ------------------------------------------------------------------ */
    return a_cache[n];
}
