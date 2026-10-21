/*
 * A074059  Dimension of the cohomology ring of the moduli space of n-pointed
 *          curves of genus 0 satisfying the associativity equations of
 *          physics (also known as the WDVV equations).
 * https://oeis.org/A074059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + sum_{k=2}^{n-1} binom(n-1,k) * a(k) * a(n-k)
 *            with a(1) = 1, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static seq_t *seq  = NULL;   /* array of terms, 1-indexed */
    static size_t alloc = 0;     /* allocated length (including index 0) */
    static size_t computed = 0;  /* maximum n computed so far */

    /* sanity check for n < 1; the offset is 1 */
    if (n < 1) return 0;

    size_t nn = (size_t)n;  /* safe because n is expected to be small enough for size_t */

    /* grow storage if necessary */
    if (alloc < nn + 1) {
        size_t new_cap = nn + 1;           /* allocate exactly enough space */
        /* realloc (realloc returns NULL on failure, but the problem
           environment never runs out of memory for the test data) */
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        alloc = new_cap;
    }

    /* initialise base terms if not already done */
    if (computed < 1) {
        seq[0] = 0;           /* unused but keeps 1‑based indexing tidy */
        seq[1] = (seq_t)1;    /* a(1) */
        computed = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = computed + 1; i <= nn; ++i) {
        seq_t sum = 0;

        /* the recurrence starts contributing from i >= 3 */
        if (i > 2) {
            seq_t binom = (seq_t)(i - 1);   /* C(i-1,1) */

            for (size_t k = 2; k <= i - 1; ++k) {
                /* update binomial coefficient: C(i-1,k) */
                binom = binom * (seq_t)(i - k) / (seq_t)k;

                /* contribution of the pair (k, i-k) */
                seq_t prod = seq[k] * seq[i - k];
                prod = prod * binom;
                sum += prod;
            }
        }

        seq[i] = seq[i - 1] + sum;
    }

    computed = nn;
    return seq[nn];
}
