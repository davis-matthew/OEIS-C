/*
 * A101054  Expansion of e.g.f. exp(exp(x)-1)/(1-x)^2.
 * https://oeis.org/A101054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 * a(n) = Sum_{k=1..n} (2*(k-1)! + 1) * C(n-1, k-1) * a(n-k)
 */
#include <stdlib.h>
#include "../utils/resize_array.h"

/* factorial table --------------------------------------------------------- */
static seq_t *fac = NULL;   /* fac[i] = i!   */
static idx_t fac_size = 0;  /* number of stored entries */

/* ensure fac[n] is computed */
static void extend_factorial(idx_t n)
{
    if (fac_size > n)
        return;                    /* already present */

    seq_t *new_fac = resize_array(fac, fac_size, n + 1);

    if (fac_size == 0) {            /* first time – set fac[0] */
        new_fac[0] = (seq_t)1;
        fac_size = 1;
    }
    /* compute missing factorials */
    idx_t i;
    for (i = fac_size; i <= n; ++i)
        new_fac[i] = new_fac[i - 1] * (seq_t)i;

    fac = new_fac;
    fac_size = n + 1;
}

/* sequence table ----------------------------------------------------------- */
static seq_t *a_vals = NULL;   /* a_vals[i] = a(i) */
static idx_t a_size = 0;       /* number of stored entries */

/* compute a(k) for all k <= n */
static void extend_sequence(idx_t n)
{
    if (a_size > n)
        return;                    /* already present */

    /* previous size before extension */
    idx_t old_size = a_size;

    seq_t *new_a = resize_array(a_vals, a_size, n + 1);
    if (old_size == 0)
        new_a[0] = (seq_t)1;        /* a(0) = 1 */

    /* factorials needed up to n will be ready */
    extend_factorial(n);

    idx_t i, k;
    /* compute missing elements */
    for (i = (old_size == 0 ? 1 : old_size); i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t comb = (seq_t)1;       /* C(i-1, 0) */

        for (k = 1; k <= i; ++k) {
            seq_t term = ((seq_t)2 * fac[k - 1] + (seq_t)1);
            term *= comb;
            term *= new_a[i - k];
            sum += term;

            if (k < i) {
                /* comb = C(i-1, k) from C(i-1, k-1) */
                comb = comb * (seq_t)(i - k) / (seq_t)k;
            }
        }
        new_a[i] = sum;
    }

    a_vals = new_a;
    a_size = n + 1;
}

/* public entry ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    extend_sequence(n);
    return a_vals[n];
}
