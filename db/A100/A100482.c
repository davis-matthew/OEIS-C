/*
 * A100482  a(1) = 1 thereafter a(n) = Sum_{k=1..n-1} ceiling(a(n-k)/k).
 * https://oeis.org/A100482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n) = Sum_{k=1}^{n-1} ceil(a(n-k)/k) (n ≥ 1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation: store computed terms a(1)..a(size). */
    static seq_t *vals = NULL;
    static idx_t size = 0;      /* number of terms stored */
    static idx_t cap  = 0;      /* allocated capacity (elements) */

    if (n < 1)        /* undefined region – return 0 */
        return 0ULL;

    /* Initialise storage on first call. */
    if (size == 0) {
        cap   = 128;                     /* initial capacity */
        vals  = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!vals) return 0;             /* allocation error */
        vals[0] = (seq_t)1;              /* a(1) = 1 */
        size   = 1;
    }

    /* If the requested index is already cached, return it. */
    if (n <= size)
        return vals[n - 1];

    /* Compute terms up to n, extending the cache as needed. */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t sum = 0;

        for (idx_t k = 1; k < i; ++k) {
            seq_t aj = vals[(i - k) - 1];            /* a(i-k)   */
            seq_t div = (aj + (seq_t)k - 1) / (seq_t)k; /* ceil division */
            sum += div;
        }

        /* Extend capacity if the array is full. */
        if (size == cap) {
            idx_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                /* allocation failure */
            vals = tmp;
            cap  = new_cap;
        }

        vals[size++] = sum;                 /* store a(i)   */
    }

    return vals[n - 1];
}
