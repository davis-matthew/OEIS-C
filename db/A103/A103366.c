/*
 * A103366  Second column of triangle A103364, which equals the matrix inverse
 *          of the Narayana triangle (A001263).
 * https://oeis.org/A103366
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=2}^{n-1} N(n,k)·a(k) where N(n,k) =
 *            (1/n)·binom(n,k)·binom(n,k-1) a(2) = 1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 16, i128 n <= 25,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *tab = NULL;
    static idx_t computed = 1;        /* last index that has a value */

    /* initial allocation (indices 0…2) */
    if (tab == NULL) {
        tab = seq_malloc(3);
        tab[0] = (seq_t)0;
        tab[1] = (seq_t)0;
        tab[2] = (seq_t)1;
        computed = 2;
    }

    /* existing value? */
    if (n <= computed)
        return tab[n];

    /* extend storage */
    tab = seq_realloc(tab, n + 1);

    /* build values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* ⟶ compute a(i) for i ≥ 3 ------------------------------------ */
        seq_t sum = (seq_t)0;

        /* N(i,2) = binom(i,2) = i*(i-1)/2 */
        seq_t B = ((seq_t)i * (seq_t)(i - 1)) / (seq_t)2;

        for (idx_t k = 2; k <= i - 1; ++k) {
            /* subtract a(k) * N(i,k) */
            sum -= tab[k] * B;

            /* update B to N(i,k+1) for next iteration                     */
            if (k < i - 1) {
                /*   N(i,k+1) = N(i,k) * (i-k+1)(i-k) / (k(k+1))            */
                B *= (seq_t)(i - k + 1);
                B *= (seq_t)(i - k);
                B /= (seq_t)k;
                B /= (seq_t)(k + 1);
            }
        }
        tab[i] = sum;
    }

    computed = n;
    return tab[n];
}
