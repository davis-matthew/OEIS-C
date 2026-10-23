/*
 * A100938  Self-convolution of A092684.
 * https://oeis.org/A100938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} Σ_{j=0..n-k} binomial(k,j)·a(j)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1  */

#include <stdlib.h>

static seq_t *a_seq = NULL;          /* cached values a[0..last_computed]   */
static idx_t  size_allocated = 0;   /* current allocation size              */
static idx_t  last_computed = 0;    /* maximum index that has been computed */

/* Return the n‑th term of the Sequence A100938.  Calls may come in any
   order; results are memoised. */
seq_t a(idx_t n)
{
    /* Initialise storage on first call. */
    if (size_allocated == 0) {
        a_seq = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!a_seq) return (seq_t)0;          /* allocation failed */
        a_seq[0] = (seq_t)1;                  /* a(0) = 1          */
        size_allocated = 1;
        last_computed   = 0;
    }

    /* If we already know a(n), return it immediately. */
    if (n <= last_computed) return a_seq[n];

    /* Grow the array to accommodate a[0..n]. */
    if (n + 1 > size_allocated) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_seq, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;           /* re‑allocation failed */
        a_seq = tmp;
        size_allocated = new_size;
    }

    /* Compute terms from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t bin = (seq_t)1;                     /* binomial(k,0) */
            sum += a_seq[0] * bin;                   /* j = 0 term */

            for (idx_t j = 1; j <= i - k; ++j) {
                /* bin = bin * (k - j + 1) / j  (integer division) */
                bin = bin * (seq_t)(k - j + 1) / (seq_t)j;
                sum += a_seq[j] * bin;
            }
        }

        a_seq[i] = sum + (seq_t)1;                     /* a(i) = 1 + sum */
    }

    last_computed = n;
    return a_seq[n];
}
