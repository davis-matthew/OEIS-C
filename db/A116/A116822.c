/*
 * A116822  Number of permutations of length n which avoid the patterns 2341,
 *          3421, 4123.
 * https://oeis.org/A116822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A116822:
   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=241,
   a(7)=766, a(8)=2399
   For n>8:
   a(n) = 8*a(n‑1) – 26*a(n‑2) + 47*a(n‑3) – 51*a(n‑4)
          + 35*a(n‑5) – 18*a(n‑6) + 4*a(n‑7) – a(n‑8)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coefficients c[k] = -d_k from the denominator of the GF
       (k = 1..8).  The recurrence is
          a[n] = Σ_{k=1..8} c[k]*a[n-k] + ncoeff[n]   (n ≤ 8)
    */
    static const int c[9] = {0, 8, -26, 47, -51, 35, -18, 4, -1};
    static const int ncoeff[9] = {0, 1, -6, 16, -22, 18, -12, 3, -1};

    static seq_t *cache = NULL;          /* cache[0] = 0, cache[1..] = a(n) */
    static size_t cached_size = 0;        /* number of allocated elements */

    if (n == 0) return 0;                /* offset 1: n>=1 in valid calls */

    /* Grow the cache if necessary */
    if (cached_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1; /* store up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);              /* allocation failed */
        cache = tmp;
        cached_size = new_size;
        cache[0] = 0;                   /* a(0) = 0 for the recurrence */
    }

    /* Compute any missing terms up to n */
    idx_t start = (idx_t)(cached_size - 1); /* last computed index */
    if (start < 1) start = 1;              /* ensure we start from 1 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; k <= 8 && k <= i; ++k) {
            int coeff = c[k];
            if (coeff >= 0)
                pos += (seq_t)coeff * cache[i - k];
            else
                neg += (seq_t)(-coeff) * cache[i - k];
        }
        if (i <= 8) {
            int off = ncoeff[i];
            if (off >= 0)
                pos += (seq_t)off;
            else
                neg += (seq_t)(-off);
        }
        cache[i] = pos - neg;  /* pos ≥ neg guarantees a non‑negative result */
    }

    return cache[n];
}
