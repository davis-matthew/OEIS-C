/*
 * A122777  Coefficients of L-series for elliptic curve "30a1": y^2 + x * y +
 *          y = x^3 + x + 2.
 * https://oeis.org/A122777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1500, i16 n <= 1500, i32 n <= 1500, i64 n <= 1500,
 *            i128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122777 – coefficients of η(q^3)η(q^5)η(q^6)η(q^10) –
   minus η(q)η(q^2)η(q^15)η(q^30)
   Formula: a(n) = coeff of x^n in [a1(x) – a2(x)]
   where a1(x) = x ∏_{k>0}(1−x^(3k))(1−x^(5k))(1−x^(6k))(1−x^(10k))
         a2(x) = x² ∏_{k>0}(1−x^k)(1−x^(2k))(1−x^(15k))(1−x^(30k)).
*/

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* helpers to support both C and C++ with arbitrary‑precision seq_t */
#ifdef __cplusplus
static inline seq_t *alloc_seq(size_t len)
{
    return new seq_t[len];
}
static inline void free_seq(seq_t *p, size_t /*len*/)
{
    delete[] p;
}
#else
static inline seq_t *alloc_seq(size_t len)
{
    return (seq_t*)calloc(len, sizeof(seq_t));
}
static inline void free_seq(seq_t *p, size_t /*len*/)
{
    free(p);
}
#endif

/* product ∏_{j=1}^∞ (1−x^{m j}) expands as
 * Σ_{t=-∞}^{∞} (−1)^t x^{m·t(3t−1)/2}   (Euler pentagonal theorem).
 * We generate this series up to exponent max_n (inclusive) and
 * convolve it with the current accumulated product.
 */
static void compute_product(idx_t max_n,
                            const idx_t mods[4], size_t mod_cnt,
                            seq_t *out)
{
    /* initialise output to zero, constant term 1 */
    for (idx_t i = 0; i <= max_n; ++i) out[i] = 0;
    out[0] = 1;

    /* temporary array used for convolution */
    seq_t *tmp = alloc_seq((size_t)max_n + 1);

    for (size_t mi = 0; mi < mod_cnt; ++mi) {
        idx_t m = mods[mi];

        /* build sparse factor corresponding to ∏_{k≥1}(1−x^(m k)) */
        idx_t exp[2000];
        seq_t  coef[2000];
        size_t tcnt = 0;
        exp[tcnt] = 0;          /* constant term */
        coef[tcnt] = 1;
        ++tcnt;

        for (idx_t k = 1;; ++k) {
            idx_t e1 = m * k * (3 * k - 1) / 2;
            if (e1 > max_n) break;
            exp[tcnt] = e1;
            coef[tcnt] = (k & 1) ? (seq_t)-1 : (seq_t)1;
            ++tcnt;

            idx_t e2 = m * k * (3 * k + 1) / 2;
            if (e2 <= max_n) {
                exp[tcnt] = e2;
                coef[tcnt] = (k & 1) ? (seq_t)-1 : (seq_t)1;
                ++tcnt;
            }
        }

        /* convolution of current product with this sparse factor */
        for (idx_t i = 0; i <= max_n; ++i) tmp[i] = 0;
        for (idx_t i = 0; i <= max_n; ++i) {
            seq_t vi = out[i];
            if (vi == 0) continue;
            for (size_t t = 0; t < tcnt; ++t) {
                idx_t j = i + exp[t];
                if (j > max_n) continue;
                tmp[j] += vi * coef[t];
            }
        }
        /* copy back to out for next iteration */
        for (idx_t i = 0; i <= max_n; ++i) out[i] = tmp[i];
    }

    free_seq(tmp, (size_t)max_n + 1);
}

/* pre‑computed basis products */
static const idx_t MODS1[4] = {3, 5, 6, 10};
static const idx_t MODS2[4] = {1, 2, 15, 30};

static seq_t *P1 = NULL, *P2 = NULL;
static idx_t  P1_len = 0,  P2_len = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    /* extend cached products if needed */
    if ((idx_t)(n + 1) > P1_len) {
        if (P1) free_seq(P1, P1_len);
        P1 = alloc_seq((size_t)(n + 1));
        compute_product(n, MODS1, 4, P1);
        P1_len = (idx_t)(n + 1);
    }
    if ((idx_t)(n + 1) > P2_len) {
        if (P2) free_seq(P2, P2_len);
        P2 = alloc_seq((size_t)(n + 1));
        compute_product(n, MODS2, 4, P2);
        P2_len = (idx_t)(n + 1);
    }

    seq_t term1 = P1[n - 1];
    seq_t term2 = (n >= 2) ? P2[n - 2] : (seq_t)0;
    return term1 - term2;
}
