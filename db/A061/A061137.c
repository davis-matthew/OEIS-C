/*
 * A061137  Number of degree-n odd permutations of order dividing 6.
 * https://oeis.org/A061137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A061137
 * Number of degree-n odd permutations of order dividing 6.
 * The sequence satisfies the recurrence:
 *   e[0] = 1, o[0] = 0 (even/odd counts)
 *   for n ≥ 1:
 *     e[n] = Σ_{L∈{1,2,3,6}, L≤n} (∏_{k=1}^{L-1} (n-k))
 *               * (L odd  ? e[n-L] : o[n-L])
 *     o[n] = Σ_{L∈{1,2,3,6}, L≤n} (∏_{k=1}^{L-1} (n-k))
 *               * (L odd  ? o[n-L] : e[n-L])
 *   a(n) = o[n]
 *
 * Here the product over k is the falling factorial (n-1)(n-2)...(n-L+1).
 * The recursion is computed in O(n) time with O(n) memory.
 */

#include <stdlib.h>

static seq_t *e = NULL;   /* counts of even permutations */
static seq_t *o = NULL;   /* counts of odd permutations  */
static idx_t  max_n = 0;  /* highest n computed */

static const idx_t cycles[4] = {1, 2, 3, 6};

seq_t a(idx_t n)
{
    /* initialise tables for n = 0 if not yet done */
    if (!e || !o) {
        size_t init = 1; /* store element 0 */
        e = (seq_t *)malloc(init * sizeof(seq_t));
        o = (seq_t *)malloc(init * sizeof(seq_t));
        if (!e || !o) return (seq_t)0;  /* allocation failed */
        e[0] = (seq_t)1;
        o[0] = (seq_t)0;
        max_n = 0;
    }

    /* extend precomputed values up to n */
    if (n > max_n) {
        size_t old_len = (size_t)(max_n + 1);
        size_t new_len = (size_t)(n + 1);
        e = (seq_t *)realloc(e, new_len * sizeof(seq_t));
        o = (seq_t *)realloc(o, new_len * sizeof(seq_t));
        if (!e || !o) return (seq_t)0;  /* reallocation failed */

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum_even = (seq_t)0;
            seq_t sum_odd  = (seq_t)0;

            for (int j = 0; j < 4; ++j) {
                idx_t L = cycles[j];
                if (L > i) continue;

                /* compute falling factorial (i-1)(i-2)...(i-L+1) */
                seq_t factor = (seq_t)1;
                for (idx_t k = 1; k < L; ++k)
                    factor *= (seq_t)(i - k);

                if (L & 1) {  /* odd L: sign unchanged */
                    sum_even += factor * e[i - L];
                    sum_odd  += factor * o[i - L];
                } else {      /* even L: sign toggles */
                    sum_even += factor * o[i - L];
                    sum_odd  += factor * e[i - L];
                }
            }

            e[i] = sum_even;
            o[i] = sum_odd;
        }

        max_n = n;
    }

    return o[n];
}
