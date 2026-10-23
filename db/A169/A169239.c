/*
 * A169239  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169239: Number of reduced words of length n in the Coxeter group on 34 generators with (S_i)^2 = (S_i S_j)^28 = I.
   Recurrence:
     a(0) = 1
     for 1 ≤ n ≤ 27:  a(n) = 32 * Σ_{k=1}^{n} a(n-k) + 2
     for n = 28:      a(28) = 32 * Σ_{k=1}^{27} a(28-k) – 528 * a(0) + 1
     for n ≥ 29:      a(n) = 32 * Σ_{k=1}^{27} a(n-k) – 528 * a(n-28)
   (Coefficients come from the rational generating function.)
*/

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t cache_cap = 0;      /* current capacity of seq_cache */
static idx_t  last_computed = 0xFFFFFFFFFFFFFFFFULL; /* sentinel for "none" */

/* Compute / retrieve a(n).  Uses memoisation. */
seq_t a(idx_t n)
{
    /* Allocate or grow cache to hold indices up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – caller must handle */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Base case a(0) */
    if (last_computed == 0xFFFFFFFFFFFFFFFFULL || last_computed == 0) {
        seq_cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i >= 27) ? 27 : i;  /* number of terms to sum over */
        for (idx_t k = 1; k <= limit; ++k)
            sum += ((seq_t)32) * seq_cache[i - k];

        if (i >= 29) {
            seq_cache[i] = sum - ((seq_t)528) * seq_cache[i - 28];
        } else if (i == 28) {
            seq_cache[i] = sum - ((seq_t)528) * seq_cache[0] + (seq_t)1;
        } else { /* 1 ≤ i ≤ 27 */
            seq_cache[i] = sum + (seq_t)2;
        }
    }

    last_computed = n;
    return seq_cache[n];
}
