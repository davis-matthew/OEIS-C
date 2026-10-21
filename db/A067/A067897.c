/*
 * A067897  a(n) = A000085(n) - (1 + Sum_{j=1..n-1} A000085(j)).
 * https://oeis.org/A067897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000085(n) - ∑_{j=0}^{n-1} A000085(j)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for involution counts (A000085) and their prefix sums */
    static seq_t *I      = NULL;   /* I[i] = A000085(i)     */
    static seq_t *prefix = NULL;   /* prefix[i] = Σ_{j=0}^i I[j] */
    static idx_t  max_n  = (idx_t)-1;
    static size_t cap    = 0;

    /* a(0) = 0 by definition of the sequence */
    if (n == 0) {
        return (seq_t)0;
    }

    /* first‑time initialization: compute values for i=0,1 */
    if (max_n == (idx_t)-1) {
        cap = 2;                           /* enough for indices 0 and 1 */
        I      = (seq_t *)malloc(cap * sizeof(seq_t));
        prefix = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!I || !prefix) {
            /* allocation failed – return 0 (unlikely on valid builds) */
            return (seq_t)0;
        }
        I[0] = (seq_t)1;      /* A000085(0) = 1 */
        I[1] = (seq_t)1;      /* A000085(1) = 1 */
        prefix[0] = I[0];                    /* prefix sum up to 0 */
        prefix[1] = prefix[0] + I[1];        /* prefix sum up to 1 */
        max_n = 1;
    }

    /* extend the cache if the requested index is beyond what we have */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmpI  = (seq_t *)realloc(I,     new_cap * sizeof(seq_t));
            seq_t *tmpP  = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
            if (!tmpI || !tmpP) {
                /* reallocation failed – return 0 */
                return (seq_t)0;
            }
            I      = tmpI;
            prefix = tmpP;
            cap    = new_cap;
        }
        /* compute values from max_n+1 up to n (inclusive) */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* recurrence: I[i] = I[i-1] + (i-1)*I[i-2]   (for i>=2) */
            seq_t term1 = I[i - 1];
            seq_t term2 = I[i - 2] * (seq_t)(i - 1);
            I[i] = term1 + term2;
            prefix[i] = prefix[i - 1] + I[i];
        }
        max_n = n;
    }

    /* result: I[n] - prefix[n-1] (for n>=1) */
    seq_t result = I[n] - prefix[n - 1];
    return result;
}
