/*
 * A146163  Expansion of q^(-3/4) * eta(q^2)^2 * eta(q^20) / (eta(q)^2 *
 *          eta(q^4)) in powers of q.
 * https://oeis.org/A146163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in q^(-3/4) * eta(q^2)^2 * eta(q^20) /
 *            (eta(q)^2 * eta(q^4))
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 100, u64 n <= 332,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented via the Euler product with a period‑20 exponent sequence.
   The sequence of exponents c_k (k ≥ 1) is periodic 20:
     2,0,2,1,2,0,2,1,2,0,2,1,2,0,2,1,2,0,2,0, …
   The generating function is
     ∑_{n≥0} a(n) q^n = ∏_{k≥1} (1 - q^k)^(-c_k).
   The coefficients are computed by a standard unbounded‑coin DP.
*/

#include <stdlib.h>

static const int period20[20] = {
    2,0,2,1,2,0,2,1,2,0,2,1,2,0,2,1,2,0,2,0
};

/* The function that returns the n‑th term of sequence A146163. */
seq_t a(idx_t n)
{
    /* Target index for the coefficient array. */
    idx_t target = 4 * n + 3;  /* this line keeps the code equivalent
     * to the OEIS description, but the sequence itself equals a[n].
     * However the requested index by the user is n, so the coefficient
     * at position n is returned.  The value 4*n+3 is only used for
     * compatibility with the “a(n)=A146162(4*n+3)” remark.  */
    /* Use the coefficient as the requested term (no offset). */
    /* We compute up to the maximum requested index. */
    static seq_t *coeff = NULL;
    static idx_t coeff_len = 0;   /* number of allocated elements */
    static idx_t current_max = 0; /* highest index computed */

    if (target > current_max) {
        /* Allocate or enlarge storage. */
        idx_t needed = target + 1;
        seq_t *newptr = (seq_t *)realloc(coeff, needed * sizeof(seq_t));
        if (!newptr) return 0;   /* allocation failed */
        coeff = newptr;
        /* Initialise newly allocated part to zero. */
        for (idx_t i = coeff_len; i < needed; ++i)
            coeff[i] = (seq_t)0;
        coeff_len = needed;

        /* Reset and recompute the whole array from scratch. */
        for (idx_t i = 0; i < coeff_len; ++i)
            coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;

        for (idx_t k = 1; k <= target; ++k) {
            int r = period20[(k - 1) % 20];
            for (int rep = 0; rep < r; ++rep) {
                for (idx_t j = k; j <= target; ++j) {
                    coeff[j] += coeff[j - k];
                }
            }
        }
        current_max = target;
    }

    return coeff[n];
}
