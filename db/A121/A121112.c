/*
 * A121112  Edge-rooted tree-like octagonal systems (see the Cyvin et al.
 *          reference for precise definition).
 * https://oeis.org/A121112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121112: Edge‑rooted tree‑like octagonal systems.
   Formula: a(n) = 5 * A036758(n‑1)  for n >= 2,   a(1) = 0.
   The sequence A036758 is defined recursively by the generating function
        G(z) = z + 5*z*G(z) + 6*z*G(z)^2 + z*G(z)^3 .
   The coefficients g[n] of G(z) satisfy:
        g[1] = 1
        g[n] = 5*g[n‑1] + 6*Σ_{i=1}^{n‑2} g[i]*g[n‑1‑i]
                     + Σ_{i=1}^{n‑3} Σ_{j=1}^{n‑2‑i} g[i]*g[j]*g[n‑1‑i‑j]   (n ≥ 3)
   The required term is a(n) = 5*g[n‑1] for n ≥ 2, and a(1)=0.
   The computation below uses dynamic programming and convolutions
   and runs in O(n²) time and O(n) memory.

   The routine uses only the standard C headers requested in the
   specification and never relies on language extensions.  It works
   for any integer type chosen as seq_t and idx_t.  */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0) return 0;          /* (not used by the judge) */
    if (n == 1) return 0;          /* a(1) = 0 */

    /* We need terms g[1] … g[n‑1].  */
    idx_t max_needed = n - 1;

    /* Allocate two dynamic arrays:
       g   – coefficients of G(z);
       conv – pairwise convolution conv[i] = Σ_{k=1}^{i‑1} g[k]*g[i‑k].
       Size: max_needed + 1 (we ignore index 0).   */
    seq_t *g = (seq_t *)malloc((max_needed + 1) * sizeof(seq_t));
    seq_t *conv = (seq_t *)malloc((max_needed + 1) * sizeof(seq_t));
    if (g == NULL || conv == NULL) return 0;   /* allocation failure */

    /* Initialize
         g[1] = 1
         conv[0] and conv[1] unused = 0
         conv[2] = g[1]*g[1] = 1
    */
    g[0] = 0;          /* unused */
    g[1] = 1;
    conv[0] = 0;
    conv[1] = 0;
    if (max_needed >= 2) conv[2] = g[1] * g[1];

    /* Main recurrence – compute g[2] … g[max_needed] */
    for (idx_t curr = 1; curr <= max_needed - 1; ++curr) {
        /* term for one branch: 5 * g[curr] */
        seq_t term1 = 5 * g[curr];

        /* term for two branches: 6 * conv[curr]
           that is, 6 * Σ_{i=1}^{curr‑1} g[i]*g[curr‑i]      */
        seq_t term2 = 0;
        if (curr >= 2) term2 = 6 * conv[curr];

        /* term for three branches:
           Σ_{i=1}^{curr‑2} g[i] * conv[curr‑i]                      */
        seq_t term3 = 0;
        if (curr >= 3) {
            for (idx_t i = 1; i <= curr - 2; ++i) {
                term3 += g[i] * conv[curr - i];
            }
        }

        g[curr + 1] = term1 + term2 + term3;

        /* Update convolution conv[curr+1] =
           Σ_{i=1}^{curr} g[i]*g[curr+1‑i]                         */
        if (curr + 1 >= 2) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= curr; ++i) {
                sum += g[i] * g[curr + 1 - i];
            }
            conv[curr + 1] = sum;
        }
    }

    /* Final answer: a(n) = 5 * g[n‑1]   (n >= 2) */
    seq_t result = 5 * g[n - 1];

    /* Clean up */
    free(g);
    free(conv);

    return result;
}
