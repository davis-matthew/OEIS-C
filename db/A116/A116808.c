/*
 * A116808  Number of permutations of length n which avoid the patterns 1342,
 *          3214, 4312.
 * https://oeis.org/A116808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A116808: Number of permutations of length n which avoid the patterns
 * 1342, 3214, 4312.
 *
 * Recurrence (for n ≥ 10):
 *   a(n) = 9*a(n-1) - 34*a(n-2) + 69*a(n-3) - 78*a(n-4) + 41*a(n-5)
 *          + 6*a(n-6) - 21*a(n-7) + 11*a(n-8) - 2*a(n-9)
 *
 * Initial values:
 *   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=71,
 *   a(6)=213, a(7)=569, a(8)=1389, a(9)=3175.
 *
 * The implementation memoises computed values and can answer calls in any
 * order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* One‑based indexing: a(1) is the first term. */
    if (n < 1)
        return (seq_t)0;

    static seq_t *seq = NULL;   /* memoised values */
    static idx_t cached = 0;    /* highest n computed so far */

    /* Initial hard‑coded terms (including a(0)=0 for recurrence). */
    static const seq_t init[10] = {
        (seq_t)0, /* a(0) unused, but set to 0 */
        (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21,
        (seq_t)71, (seq_t)213, (seq_t)569,
        (seq_t)1389, (seq_t)3175
    };

    /* Allocate or grow the memoisation array as needed. */
    if (seq == NULL || n > cached) {
        /* New size: at least n+1 (indices 0..n). */
        seq_t *new_seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (new_seq == NULL)  /* allocation failure */ {
            return (seq_t)0;
        }

        /* Copy previously cached values if any. */
        if (seq != NULL) {
            for (idx_t i = 1; i <= cached; ++i)
                new_seq[i] = seq[i];
        }

        /* Initialise the first 9 terms (or up to cached). */
        if (cached == 0) { /* first run */
            for (idx_t i = 0; i <= 9 && i <= n; ++i)
                new_seq[i] = init[i];
        }

        /* Compute remaining terms up to n. */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 9) {
                new_seq[i] = init[i];  /* directly from table */
            } else {
                /* Recurrence: avoid underflow by separating positives
                 * and negatives, all arithmetic done in seq_t. */
                seq_t pos = 0, neg = 0;
                pos += (seq_t)9  * new_seq[i-1];
                pos += (seq_t)69 * new_seq[i-3];
                pos += (seq_t)41 * new_seq[i-5];
                pos += (seq_t)6  * new_seq[i-6];
                pos += (seq_t)11 * new_seq[i-8];
                neg += (seq_t)34 * new_seq[i-2];
                neg += (seq_t)78 * new_seq[i-4];
                neg += (seq_t)21 * new_seq[i-7];
                neg += (seq_t)2  * new_seq[i-9];
                new_seq[i] = pos - neg; /* pos ≥ neg for valid values */
            }
        }

        /* Replace old array.  The old buffer is unused for this problem
         * (no memory pressure handling), just leak it. */
        seq = new_seq;
        cached = n;
    }

    return seq[n];
}
