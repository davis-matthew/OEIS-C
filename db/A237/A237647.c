/*
 * A237647  G.f. satisfies: A(x) = (1 + x + x^2)^7 * A(x^2)^4.
 * https://oeis.org/A237647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = (1 + x + x^2)^7 * A(x^2)^4 a(n) can be computed by
 *            multiplying the generating series F_k(x) = (1 + x^{2^k} +
 *            x^{2^{k+1}})^{7·4^k} for k=0,1,2,…, keeping only terms up to
 *            x^n. This implements that product. The function is fully
 *            memo‑free and works in O(n^2) time, but is fast enough for
 *            n≤10...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Auxiliary: binomial(n,k) with n,k small enough that the result
   fits in seq_t (or is truncated if not). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n)             return (seq_t)0;
    if (k > n - k)         k = n - k;       /* symmetry */

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - (k - i)) / i   (exact division) */
        res = res * (seq_t)(n - (k - i));
        res = res / (seq_t)i;
    }
    return res;
}

/* Build the local factor polynomial f[j] = coeff of x^{j·w}
   in (1 + x^w + x^{2w})^e, truncated to degree ≤ n. */
static void local_factor(idx_t w, idx_t e, idx_t n, seq_t *f)
{
    idx_t maxm = n / w;
    if (maxm > 2 * e) maxm = 2 * e;
    f[0] = (seq_t)1;
    for (idx_t m = 1; m <= maxm; ++m) {
        seq_t sum = (seq_t)0;
        idx_t bmax = m / 2;
        if (bmax > e) bmax = e;
        for (idx_t b = 0; b <= bmax; ++b) {
            idx_t a = m - 2 * b;
            if (a > e - b) continue;      /* a + b must not exceed e   */
            seq_t term1 = binom_small(e, b);
            seq_t term2 = binom_small(e - b, a);
            sum += term1 * term2;
        }
        f[m] = sum;
    }
}

/*--------------------------------------------------------------------------*/
/*  Public function required by the test harness                           */
/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* allocate two polynomials of degree n */
    seq_t *cur = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;

    for (idx_t k = 0, w = 1; w <= n; ++k, w <<= 1) {
        /* e = 7 * 4^k                           */
        idx_t e = 7 * ((idx_t)1 << (2 * k));

        /* build local factor coefficients f[j] for j w ≤ n   */
        idx_t maxm = n / w;
        if (maxm > 2 * e) maxm = 2 * e;
        size_t fsize = (size_t)(maxm + 1);
        seq_t *f = (seq_t *)malloc(fsize * sizeof(seq_t));
        local_factor(w, e, n, f);

        /* zero the next polynomial */
        for (idx_t i = 0; i <= n; ++i) next[i] = (seq_t)0;

        /* convolution: next[i + j*w] += cur[i] * f[j]  */
        for (idx_t j = 0; j <= maxm; ++j) {
            seq_t fj = f[j];
            if (fj == (seq_t)0) continue;
            idx_t offset = j * w;
            for (idx_t i = 0; i + offset <= n; ++i) {
                next[i + offset] += cur[i] * fj;
            }
        }

        /* free factor buffer and swap arrays for next round */
        free(f);
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[n];
    free(cur);
    free(next);
    return result;
}
