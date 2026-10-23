/*
 * A124027  G. J. Chaitin's numbers of s-expressions of size n are given by
 *          the coefficients of polynomials p(k, x) satisfying p(k, x) =
 *          Sum[p(j, x)*p(k - j, x), {j, 2, k - 1}]. The coefficients of these
 *          polynomials give the triangle shown here.
 * https://oeis.org/A124027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   Let p(0,x) = 0, p(1,x) = x, p(2,x) = 1, and for k ≥ 3 p(k,x) =
 *            Σ_{j=2}^{k-1} p(j,x) * p(k-j,x). The sequence a(n) is obtained
 *            by concatenating the coefficients of the polynomials p(k,x) in
 *            order k = 0,1,2,3,… (row‑major order). a(1) corresponds to the
 *            first coefficient of p(0,x)
 * Range:     u8 n <= 51, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static data structures for memoisation */
    static seq_t **polys = NULL;          /* array of pointers to coefficient arrays */
    static size_t *poly_len = NULL;       /* length of each polynomial */
    static size_t polys_cap = 0;          /* capacity of polys array */
    static size_t polys_len = 0;          /* number of polynomials stored */

    static seq_t *flat = NULL;            /* flattened sequence */
    static size_t flat_cap = 0;           /* capacity of flat array */
    static size_t flat_len = 0;           /* number of terms generated */

    /* generate data until we have at least n terms */
    while (flat_len < (size_t)n) {
        idx_t k = (idx_t)polys_len;       /* next polynomial index to compute */

        /* grow polys array if necessary */
        if (polys_len == polys_cap) {
            size_t new_cap = polys_cap ? polys_cap * 2 : 4;
            polys       = (seq_t**)realloc(polys,       new_cap * sizeof(seq_t*));
            poly_len    = (size_t*)realloc(poly_len,    new_cap * sizeof(size_t));
            polys_cap   = new_cap;
        }

        /* determine length of p(k) */
        size_t len;
        if (k == 0)      len = 1;        /* p(0) = 0 */
        else if (k == 1) len = 2;        /* p(1) = x */
        else if (k == 2) len = 1;        /* p(2) = 1 */
        else if (k == 3) len = 2;        /* result of the recurrence */
        else             len = (size_t)k - 1;   /* for k ≥ 4 */

        /* allocate and zero the coefficient array */
        seq_t *coeff = (seq_t*)malloc(len * sizeof(seq_t));
        for (size_t i = 0; i < len; ++i) coeff[i] = 0;

        /* base cases */
        if (k == 0) {
            coeff[0] = 0;
        } else if (k == 1) {
            coeff[0] = 0;
            coeff[1] = 1;
        } else if (k == 2) {
            coeff[0] = 1;
        } else {
            /* accumulate convolution sums */
            for (idx_t j = 2; j <= k - 1; ++j) {
                seq_t *a = polys[(size_t)j];
                size_t len_a = poly_len[(size_t)j];
                seq_t *b = polys[(size_t)(k - j)];
                size_t len_b = poly_len[(size_t)(k - j)];

                for (size_t ia = 0; ia < len_a; ++ia) {
                    seq_t ai = a[ia];
                    for (size_t ib = 0; ib < len_b; ++ib) {
                        coeff[ia + ib] += ai * b[ib];
                    }
                }
            }
        }

        /* store polynomial */
        polys[(size_t)k]     = coeff;
        poly_len[(size_t)k]  = len;
        ++polys_len;

        /* append to flattened sequence */
        if (flat_len + len > flat_cap) {
            size_t new_cap = flat_cap ? flat_cap * 2 : 128;
            flat = (seq_t*)realloc(flat, new_cap * sizeof(seq_t));
            flat_cap = new_cap;
        }
        memcpy(flat + flat_len, coeff, len * sizeof(seq_t));
        flat_len += len;
    }

    /* retrieve the requested term (offset 1) */
    return flat[(size_t)n - 1];
}
