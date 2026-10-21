/*
 * A090628  Square array T(n,k) (row n, column k) read by antidiagonals
 *          defined by: T(n,k) is the permanent of the n X n matrix with 1 on
 *          the diagonal and k elsewhere; T(0,k)=1.
 * https://oeis.org/A090628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Σ_{j=0..p} C(p,j) · D_j · k^j , where p + k = d and
 *            d*(d+1)/2 > n. D_j is the j‑th derangement number. This
 *            implements the OEIS A090628
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 98, u64 n <= 202,
 *            u128 n <= 456, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *der = NULL;          /* derangement numbers */
static size_t der_cap = 0;         /* allocated capacity */

static void ensure_der_size(size_t needed)
{
    if (needed < der_cap)
        return;
    /* expand to needed+1 to include index 'needed' */
    size_t new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(der, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    der = tmp;
    /* compute new entries */
    if (der_cap == 0) {
        der[0] = (seq_t)1;                 /* D_0 = 1 */
    }
    if (der_cap <= 1 && needed >= 1) {
        der[1] = (seq_t)0;                 /* D_1 = 0 */
    }
    for (size_t j = der_cap; j <= needed; ++j) {
        if (j == 0) {
            der[j] = (seq_t)1;
        } else if (j == 1) {
            der[j] = (seq_t)0;
        } else {
            der[j] = (seq_t)(j - 1) * (der[j - 1] + der[j - 2]);
        }
    }
    der_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* find diagonal d with d*(d+1)/2 > n  */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* start index of diagonal d  */
    idx_t start = d * (d + 1) / 2;
    idx_t offset = n - start;      /* 0 <= offset <= d */
    idx_t p = offset;              /* row index */
    idx_t k = d - offset;          /* column index */

    /* T(0,k) = 1 for any k */
    if (p == 0)
        return (seq_t)1;

    ensure_der_size((size_t)p);    /* ensure D_j up to p */

    /* compute Σ_{j=0..p} C(p,j) * D_j * k^j */
    seq_t sum    = (seq_t)0;
    seq_t binom  = (seq_t)1;       /* C(p,0) */
    seq_t power  = (seq_t)1;       /* k^0 */

    for (idx_t j = 0; j <= p; ++j)
    {
        /* term = binom * D_j * power */
        seq_t term = binom * der[j];
        term = term * power;
        sum += term;

        /* prepare next binomial coefficient and power */
        if (j < p) {
            binom = binom * (seq_t)(p - j) / (seq_t)(j + 1);
            power = power * (seq_t)k;
        }
    }
    return sum;
}
