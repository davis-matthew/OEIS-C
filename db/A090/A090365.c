/*
 * A090365  Shifts 1 place left under the INVERT transform of the BINOMIAL
 *          transform of this sequence.
 * https://oeis.org/A090365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} b[k‑1] · a[n‑k] with b[n] = Σ_{j=0..n} C(n,j)
 *            · a[j] and b[0]=a[0]=1. This recurrence follows from
 *            A(x)=1/(1‑x·B(x)) with B(x)=A(x/(1‑x))/(1‑x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* static storage for memoised values */
static seq_t *a_arr = NULL;      /* a[n] */
static seq_t *b_arr = NULL;      /* b[n] */
static idx_t  max_n  = 0;        /* current highest computed index */

/* ensure that a_arr and b_arr contain values up to index n */
static void ensure_size(idx_t n)
{
    if (max_n > n)
        return;                  /* already have all required values */

    idx_t old = max_n;           /* number of elements previously computed */
    max_n = n + 1;               /* new required capacity */

    a_arr = seq_realloc(a_arr, old, max_n);
    b_arr = seq_realloc(b_arr, old, max_n);

    if (old == 0) {              /* initialise first term a(0)=b(0)=1 */
        a_arr[0] = (seq_t)1;
        b_arr[0] = (seq_t)1;
        old      = 1;           /* start computing from index 1 */
    }

    /* compute new values iteratively */
    for (idx_t i = old; i <= n; ++i) {

        /* compute a[i] = Σ_{k=1..i} b[k‑1] · a[i‑k] */
        seq_t sum_a = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
            sum_a += b_arr[k - 1] * a_arr[i - k];
        a_arr[i] = sum_a;

        /* compute b[i] = Σ_{j=0..i} C(i,j) · a[j]  (multiplicative binomial) */
        seq_t binom = (seq_t)1;
        seq_t sum_b = (seq_t)0;
        for (idx_t j = 0; j <= i; ++j) {
            sum_b += binom * a_arr[j];
            if (j < i)                     /* update binomial coefficient */
                binom = binom * (seq_t)(i - j) / (seq_t)(j + 1);
        }
        b_arr[i] = sum_b;
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    ensure_size(n);
    return a_arr[n];
}
