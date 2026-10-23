/*
 * A170270  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed from the rational generating function * F(t) =
 *            (t^41 + 2*t^40 + … + 2*t + 1) / * (190*t^41 - 19*t^40 - … - 19*t
 *            + 1). * * Recurrence (for all n ≥ 0): * a(n) = N(n) + 19 *
 *            Σ_{k=1}^{40} a(n-k) – 190 * a(n-41) * where N(n) = 1 (n = 0), 2
 *            (1 ≤ n ≤ 40), 1 (n = 41), else 0. * * This recur...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary‑order calls efficiently */
    static seq_t *cached = NULL;
    static idx_t cached_len = 0;      /* number of elements cached (size) */

    /* initialise cache on first call */
    if (cached_len == 0) {
        cached_len = 1;
        cached = (seq_t *)malloc(sizeof(seq_t));
        cached[0] = 1;    /* a(0) = 1 */
    }

    /* extend cache if necessary */
    if (n >= cached_len) {
        size_t new_len = (size_t)(n + 1);
        cached = (seq_t *)realloc(cached, new_len * sizeof(seq_t));

        for (idx_t i = cached_len; i <= n; ++i) {
            /* numerator coefficient N(i) */
            seq_t N;
            if (i == 0)          N = 1;
            else if (i <= 40)    N = 2;
            else if (i == 41)    N = 1;
            else                 N = 0;

            /* sum of the previous 40 terms (a(i-1) … a(i-40)) */
            seq_t sumPrev = 0;
            for (idx_t k = 1; k <= 40 && i >= k; ++k) {
                sumPrev += cached[i - k];
            }

            /* compute a(i) using the recurrence */
            seq_t val = N + 19 * sumPrev;
            if (i >= 41) {
                val -= 190 * cached[i - 41];
            }
            cached[i] = val;
        }
        cached_len = (idx_t)new_len;
    }

    return cached[n];
}
