/*
 * A200839  Number of 0..n arrays x(0..3) of 4 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>5, with initial terms a(1)=16, a(2)=69, a(3)=194, a(4)=435,
 *            a(5)=846
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms;  index n corresponds to
       seq[n-1] (1‑based). */
    static seq_t *seq = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */

    if (n == 0) return 0; /* undefined for n<1, return 0 just in case */

    /* Initialize cache on first call with the first five terms */
    if (seq == NULL) {
        /* allocate space for at least 5 terms */
        seq = (seq_t *)malloc(5 * sizeof(seq_t));
        seq[0] = (seq_t)16;
        seq[1] = (seq_t)69;
        seq[2] = (seq_t)194;
        seq[3] = (seq_t)435;
        seq[4] = (seq_t)846;
        cache_size = 5;
    }

    /* If the requested term is already cached, return it immediately */
    if (n <= cache_size)
        return seq[n - 1];

    /* Resize the cache to hold up to n terms */
    seq_t *new_seq = (seq_t *)realloc(seq, (size_t)n * sizeof(seq_t));
    if (new_seq == NULL) { /* allocation failed – fallback to recompute? */
        /* For robustness, free existing cache and recompute from scratch */
        free(seq);
        seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        seq[0] = (seq_t)16;
        seq[1] = (seq_t)69;
        seq[2] = (seq_t)194;
        seq[3] = (seq_t)435;
        seq[4] = (seq_t)846;
        cache_size = 5;
        new_seq = seq;
    }
    seq = new_seq;

    /* Compute all missing terms up to n using the linear recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* recurrence: a(k) = 5*a(k-1) - 10*a(k-2) + 10*a(k-3) - 5*a(k-4) + a(k-5) */
        seq_t val = 5 * seq[k - 2]           /* 5*a(k-1) */
                  - 10 * seq[k - 3]          /* -10*a(k-2) */
                  + 10 * seq[k - 4]          /* +10*a(k-3) */
                  - 5 * seq[k - 5]           /* -5*a(k-4) */
                  + seq[k - 6];              /* +a(k-5) */
        seq[k - 1] = val;
    }
    cache_size = n;
    return seq[n - 1];
}
