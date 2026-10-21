/*
 * A053849  Number of n X n matrices over GF(3) of order dividing 5 (i.e.,
 *          number of solutions of X^5=I in GL(n,3)).
 * https://oeis.org/A053849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{b=0}^{⌊n/4⌋} (∏_{k=n–4b+1}^{n} (3^k–1)·3^{k–1}) ÷
 *            (∏_{t=1}^{b} (81^t–1)·81^{t–1})
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t maxb = n / 4;

    /* pre‑compute powers of 3 (up to 3^n) */
    seq_t *pow3 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow3[i] = pow3[i - 1] * ((seq_t)3);

    /* pre‑compute powers of 81 (up to 81^maxb) */
    seq_t *pow81 = (seq_t *)malloc((maxb + 1) * sizeof(seq_t));
    pow81[0] = (seq_t)1;
    for (idx_t i = 1; i <= maxb; ++i)
        pow81[i] = pow81[i - 1] * ((seq_t)81);

    /* pre‑compute GL(b,81) = ∏_{t=1}^{b} (81^t–1)·81^{t–1} */
    seq_t *GL81 = (seq_t *)malloc((maxb + 1) * sizeof(seq_t));
    GL81[0] = (seq_t)1;
    for (idx_t k = 1; k <= maxb; ++k)
        GL81[k] = GL81[k - 1] * pow81[k - 1] * (pow81[k] - (seq_t)1);

    seq_t ans = (seq_t)0;

    for (idx_t b = 0; b <= maxb; ++b) {
        seq_t term = (seq_t)1;

        /* product over k = n–4b+1 … n of (3^k–1)·3^{k–1} */
        for (idx_t k = n - 4 * b + 1; k <= n; ++k)
            term = term * pow3[k - 1] * (pow3[k] - (seq_t)1);

        term = term / GL81[b];          /* divide by |GL(b,81)| */
        ans += term;
    }

    free(pow3);
    free(pow81);
    free(GL81);

    return ans;
}
