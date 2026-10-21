/*
 * A077851  Expansion of (1-x)^(-1)/(1 - 2*x - 2*x^3).
 * https://oeis.org/A077851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A077851: Expansion of (1‑x)^(-1)/(1 - 2*x - 2*x^3).
   The inner series b(n) satisfies
        b(0)=1, b(1)=2, b(2)=4,
        b(n)=2*b(n‑1)+2*b(n‑3)   (n≥3).
   The desired term a(n) is the cumulative sum of b:
        a(n)=∑_{k=0}^n b(k).
   This implementation memoises the b and a arrays,
   expanding them only when a larger n is requested. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *b_arr = NULL;   /* coefficients of 1/(1‑2x‑2x^3)      */
    static seq_t *a_arr = NULL;   /* cumulative sums (the sequence)    */
    static idx_t max_computed = 0;/* highest index that has been computed */

    /* Reallocate storage if necessary (or on first call). */
    if (b_arr == NULL || n > max_computed) {
        seq_t *new_b = (seq_t *)realloc(b_arr, (n + 1) * sizeof(seq_t));
        seq_t *new_a = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
        if (!new_b || !new_a)   /* allocation failure → return 0 */
            return (seq_t)0;
        b_arr = new_b;
        a_arr = new_a;

        /* Determine from which index we must start computing. */
        idx_t start = (b_arr == NULL || max_computed == 0) ? 0 : max_computed + 1;

        /* Compute new values. */
        for (idx_t i = start; i <= n; ++i) {
            seq_t bi, ai;

            if (i == 0) {
                bi = (seq_t)1;
                ai = bi;
            } else if (i == 1) {
                bi = (seq_t)2;
                ai = a_arr[i - 1] + bi;
            } else if (i == 2) {
                bi = (seq_t)4;
                ai = a_arr[i - 1] + bi;
            } else {
                bi = ((b_arr[i - 1] * ((seq_t)2)) +
                      (b_arr[i - 3] * ((seq_t)2)));
                ai = a_arr[i - 1] + bi;
            }

            b_arr[i] = bi;
            a_arr[i] = ai;
        }

        max_computed = n;
    }

    return a_arr[n];
}
