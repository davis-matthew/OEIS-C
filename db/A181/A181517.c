/*
 * A181517  Number of torsion pairs in the cluster category of type A_n.
 * https://oeis.org/A181517
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of y^n in y*P(y)-y^2, where P(y) satisfies P
 *            = y + P^2*(1+P)/(1-P) ⇔ P - y + yP - 2P^2 - P^3 = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------- */

#include <stdlib.h>

/* Memoised arrays: p[k]   – coefficients of P(y)
 *                   C2[k] – convolution C2(k)=∑_{i=1}^{k-1} p_i p_{k-i}   */
static seq_t *p  = NULL;
static seq_t *C2 = NULL;
/* current allocation size (number of usable indices) */
static size_t alloc_size = 0;
/* last index for which p[k] is known (>=1) */
static idx_t last_computed = 1;

/* enlarge storage to at least need+1 elements (indices 0..need) */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)alloc_size > need)
        return;
    size_t new_size = alloc_size ? alloc_size : 16;
    while (new_size <= (size_t)(need + 1))
        new_size <<= 1;
    p  = (seq_t *)realloc(p,  new_size * sizeof(seq_t));
    C2 = (seq_t *)realloc(C2, new_size * sizeof(seq_t));
    for (size_t i = alloc_size; i < new_size; ++i) {
        p[i]  = (seq_t)0;
        C2[i] = (seq_t)0;
    }
    alloc_size = new_size;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;       /* values below the OEIS offset are irrelevant */

    /* initialise on first call */
    if (p == NULL) {
        ensure_capacity(1);    /* need indices 0 and 1 */
        p[0]  = (seq_t)0;
        p[1]  = (seq_t)1;      /* p1 = 1 */
        C2[0] = (seq_t)0;
        C2[1] = (seq_t)0;
        last_computed = 1;
    }

    idx_t target = n - 1;      /* a(n) = p_{n-1} */

    /* make sure arrays are large enough */
    ensure_capacity(target);

    /* compute coefficients up to target if not yet done */
    if (last_computed < target) {
        for (idx_t k = last_computed + 1; k <= target; ++k) {
            /* C2(k) = Σ_{i=1}^{k-1} p_i * p_{k-i} */
            seq_t c2 = (seq_t)0;
            for (idx_t i = 1; i < k; ++i)
                c2 += p[i] * p[k - i];
            C2[k] = c2;

            /* C3(k) = Σ_{i=1}^{k-2} p_i * C2(k-i) */
            seq_t c3 = (seq_t)0;
            for (idx_t i = 1; i + 1 < k; ++i)
                c3 += p[i] * C2[k - i];

            /* recurrence: p_k = c3 + 2*c2 - p_{k-1} */
            p[k] = c3 + (c2 + c2) - p[k - 1];
        }
        last_computed = target;
    }

    return p[target];
}
