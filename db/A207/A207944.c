/*
 * A207944  Expansion of Product_{i>=1} (1 + x^(2*i + 1))/(1 - x^(2*i + 1)).
 * https://oeis.org/A207944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{i>=1} (1 + x^(2*i+1))/(1 -
 *            x^(2*i+1)) This implementation uses a dynamic‑programming
 *            approach. For each odd m ≥ 3 we multiply the current generating
 *            function G(x) by the factor 1 + 2 x^m + 2 x^{2m} + … . For each
 *            residue class modulo m we maintain a running sum of...
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;   /* a(0) = 1 */

    /* Allocate two working arrays. */
    seq_t *old = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *tmp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!old || !tmp) {            /* allocation failed – return 0 as fallback   */
        free(old);
        free(tmp);
        return (seq_t)0;
    }

    old[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) old[i] = (seq_t)0;

    for (idx_t m = 3; m <= n; m += 2) {          /* iterate over odd m ≥ 3            */
        for (idx_t r = 0; r < m && r <= n; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t idx = r; idx <= n; idx += m) {
                tmp[idx] = old[idx] + (sum << 1); /* new[idx] = old[idx] + 2*sum */
                sum += old[idx];
            }
        }
        /* swap the arrays for the next iteration */
        seq_t *swap = old;
        old = tmp;
        tmp = swap;
    }

    seq_t result = old[n];
    free(tmp);
    free(old);
    return result;
}
