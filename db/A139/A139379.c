/*
 * A139379  A Jacobsthal Catalan convolution.
 * https://oeis.org/A139379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..n} Catalan(k) * Jacobsthal(n-k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan numbers: C(0)=1, C(n)=C(n-1)*(4n-2)/(n+1) for n≥1 */
/* Jacobsthal numbers: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2) for n≥2 */

#include <stdlib.h>

static seq_t *cat_seq = NULL;   /* Catalan numbers up to current size-1 */
static seq_t *jac_seq = NULL;   /* Jacobsthal numbers up to current size-1 */
static seq_t *a_seq   = NULL;   /* the sequence a(n) */
static idx_t  size_cap = 0;     /* number of elements currently stored */

static void
ensure_size(idx_t n)
{
    if (n < size_cap)
        return;

    idx_t new_cap = n + 1;
    cat_seq = (seq_t *)realloc(cat_seq, new_cap * sizeof(seq_t));
    jac_seq = (seq_t *)realloc(jac_seq, new_cap * sizeof(seq_t));
    a_seq   = (seq_t *)realloc(a_seq,   new_cap * sizeof(seq_t));

    for (idx_t i = size_cap; i < new_cap; ++i) {
        if (i == 0) {                     /* base case */
            cat_seq[i] = (seq_t)1;
            jac_seq[i] = (seq_t)0;
            a_seq[i]   = (seq_t)0;
        } else if (i == 1) {               /* base case */
            /* Catalan(1) = 1, Jacobsthal(1) = 1 */
            cat_seq[i] = (seq_t)1;
            jac_seq[i] = (seq_t)1;
            /* sum k=0..1 : C0*J1 + C1*J0 */
            a_seq[i] = cat_seq[0] * jac_seq[1] + cat_seq[1] * jac_seq[0];
        } else {
            /* Catalan recurrence */
            cat_seq[i] = cat_seq[i-1] *
                         (seq_t)(4 * (i) - 2) /
                         (seq_t)(i + 1);

            /* Jacobsthal recurrence */
            jac_seq[i] = jac_seq[i-1] + (seq_t)2 * jac_seq[i-2];

            /* compute convolution sum for a[i] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k) {
                sum += cat_seq[k] * jac_seq[i - k];
            }
            a_seq[i] = sum;
        }
    }
    size_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return a_seq[n];
}
