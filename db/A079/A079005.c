/*
 * A079005  Exponential transform of unsigned Lah-triangle |A008297(n,k)|.
 * https://oeis.org/A079005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r! / k!) * C(r‑1, k‑1) * bell(k) * where r and k are
 *            obtained from the flattened index n by * solving r(r+1)/2 ≥ n
 *            and letting k = n – r(r–1)/2. *
 *            ----------------------------------------------------------------
 *            * This implements the exponential transform of the unsigned *
 *            Lah‑triangle (O...
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binom_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/binom_idx.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for 0, but safe */

    /* -------------- determine (r,k) from linear index n -------------- */
    idx_t r = 1;
    idx_t tri = 1;                       /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }
    idx_t prevTri = (r - 1) * r / 2;
    idx_t k = n - prevTri;

    /* -------------- compute factorial ratio r! / k! -------------- */
    seq_t fact_ratio = 1;
    for (idx_t i = k + 1; i <= r; ++i) {
        fact_ratio *= (seq_t)i;          /* product (k+1)*…*r */
    }

    /* -------------- compute binomial (r-1, k-1) -------------- */
    seq_t binom_r1_k1 = (k == 1 && r == 1) ? 1 : binom_idx(r - 1, k - 1);

    /* -------------- compute Bell(k) ----------------------------------
     *  Bell numbers are generated on demand in a static array up to
     *  the required k.  The array holds B[0]..B[maxK] with B[0]=1.
     */
    static seq_t bell[400];          /* enough for k <= 392 (more than enough) */
    static size_t bell_len = 1;      /* we have B[0] only */
    static int bell_init = 0;
    if (!bell_init) {
        bell[0] = (seq_t)1;
        bell_init = 1;
    }
    while (bell_len <= k) {          /* compute missing Bell numbers */
        size_t m = bell_len;         /* new index to compute (B[m]) */
        seq_t sum = 0;
        for (size_t j = 0; j < m; ++j) {
            /* B[m] = Σ_{j=0}^{m-1} C(m-1, j) * B[j] */
            sum += binom_idx(m - 1, j) * bell[j];
        }
        bell[m] = sum;
        ++bell_len;
    }

    /* -------------- final result ------------------------------------- */
    seq_t result = fact_ratio;
    result *= binom_r1_k1;
    result *= bell[k];
    return result;
}
