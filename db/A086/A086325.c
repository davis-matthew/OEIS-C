/*
 * A086325  Let u(1)=0, u(2)=1, u(k)=u(k-1)+u(k-2)/(k-2); then a(n)=n!*u(n).
 * https://oeis.org/A086325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * !n, where !n is the subfactorial (derangement)
 *            number. Recurrence for !n (subfactorial): !0 = 1, !1 = 0, !k =
 *            (k-1) * ( !(k-1) + !(k-2) ) for k ≥ 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *der = NULL;   /* der[i] holds !i for i ≥ 0 */
static idx_t  der_size = 0; /* number of computed values in der */
static size_t der_cap   = 0;/* capacity of der */

/* Return a(n) = n * !n  (subfactorial) for n ≥ 1.  */
seq_t a(idx_t n)
{
    /* Initialize memoization array on first call */
    if (der == NULL) {
        der_cap = 2;                     /* store 0 and 1 at least */
        der = (seq_t *)malloc(der_cap * sizeof(seq_t));
        if (der == NULL) abort();
        der[0] = (seq_t)1;               /* !0 = 1   */
        der[1] = (seq_t)0;               /* !1 = 0   */
        der_size = 2;
    }

    /* Grow array if needed */
    if (n + 1 > der_cap) {
        size_t new_cap = der_cap;
        while (n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(der, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        der = tmp;
        der_cap = new_cap;
    }

    /* Compute missing subfactorials up to n */
    for (idx_t k = der_size; k <= n; ++k) {
        seq_t sum = der[k - 1] + der[k - 2];
        der[k] = ((seq_t)k - 1) * sum;  /* !k = (k-1)*( !(k-1)+!(k-2) ) */
    }
    der_size = n + 1;

    /* a(n) = n * !n  */
    return ((seq_t)n) * der[n];
}
