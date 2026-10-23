/*
 * A131795  G.f.: Product_{k>0} (1-x^(4k-1)) / (1-x^(4k-2)).
 * https://oeis.org/A131795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 52, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131795 – Euler transform of the periodic sequence
 *  b(n): 0, 1, –1, 0, 0, 1, –1, 0, … (period 4)
 *
 *  The generating function is
 *      ∏_{k>0} (1 – x^(4k-1)) / (1 – x^(4k-2)).
 *
 *  The sequence satisfies
 *      a(0) = 1
 *      a(n) = -(1/n) * Σ_{k=1..n} σ(k) * a(n-k)      (n ≥ 1)
 *
 *  where
 *      σ(k) = Σ_{d|k} d·e_d
 *      e_d = +1  if d ≡ 3 (mod 4)
 *            -1  if d ≡ 2 (mod 4)
 *            0   otherwise
 *
 *  The computation is O(n²) time and O(n) memory.
 *
 *  seq_t and idx_t are provided by the build system.
 *  The function is safe for any n ≥ 0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return (seq_t)1;

    /* Allocate arrays for σ and a[0..n] */
    seq_t *sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma)
        return (seq_t)0;          /* allocation failed */

    seq_t *an     = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!an) { free(sigma); return (seq_t)0; }

    /* Initialise sigma array */
    for (idx_t i = 0; i <= n; ++i)
        sigma[i] = 0;

    /* Compute σ(k) = Σ_{d|k} d·e_d */
    for (idx_t d = 1; d <= n; ++d) {
        int val;
        switch (d & 3) {                 /* d % 4 */
            case 2: val = -1; break;     /* e_d = -1 for d ≡ 2 (mod 4) */
            case 3: val =  1; break;     /* e_d = +1 for d ≡ 3 (mod 4) */
            default: val =  0; break;     /* e_d =  0 otherwise */
        }
        if (val == 0)
            continue;
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += (seq_t)d * (seq_t)val;
        }
    }

    /* Compute a[0..n] using the recurrence */
    an[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += sigma[k] * an[i - k];
        }
        an[i] = -(sum / (seq_t)i);      /* exact division */
    }

    seq_t result = an[n];
    free(an);
    free(sigma);
    return result;
}
