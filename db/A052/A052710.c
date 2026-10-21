/*
 * A052710  A simple context-free grammar: difference between A049140 and its
 *          convolution square.
 * https://oeis.org/A052710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052710: a(n) = coefficient of R(z)^4 where R satisfies R = z + R^2 + R^4.
   Recurrence:
          r(0)=0, r(1)=1
          s2(n) = Σ_{i=1}^{n-1} r(i) * r(n-i)            (coefficients of R^2)
          a(n)   = Σ_{i=0}^{n} s2(i) * s2(n-i)           (coefficients of R^4)
          r(n)   = s2(n) + a(n)                           for n>1
   a(0)=0. */
#include <stdlib.h>

static seq_t *r = NULL;        /* R coefficients */
static seq_t *s2 = NULL;       /* coefficients of R^2 */
static seq_t *a_arr = NULL;    /* a(n) values, i.e. R^4 coefficients */
static idx_t max_computed = 0; /* highest n computed */

static void ensure_capacity(idx_t n) {
    /* allocate or grow arrays to size n+1 */
    size_t sz = (size_t)n + 1;
    if (r == NULL) {
        r = (seq_t *)calloc(sz, sizeof(seq_t));
        s2 = (seq_t *)calloc(sz, sizeof(seq_t));
        a_arr = (seq_t *)calloc(sz, sizeof(seq_t));
        if (r == NULL || s2 == NULL || a_arr == NULL) {
            /* allocation failed; abort rather than continue */
            abort();
        }
    } else if (sz > (size_t)(max_computed + 1)) {
        r = (seq_t *)realloc(r, sz * sizeof(seq_t));
        s2 = (seq_t *)realloc(s2, sz * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, sz * sizeof(seq_t));
        if (r == NULL || s2 == NULL || a_arr == NULL) abort();
    }
    /* r[0]=0 and r[1]=1 are needed for all n>=1 */
    if (n >= 1) r[1] = (seq_t)1;
    /* zeros already from calloc; new entries are untouched but we set them to zero */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        r[i] = 0;
        s2[i] = 0;
        a_arr[i] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* ensure a[0] exists */
        if (r == NULL) ensure_capacity(0);
        return a_arr[0];
    }
    if (n > max_computed) {
        /* Grow arrays to needed size */
        ensure_capacity(n);
        /* Compute new terms from max_computed+1 up to n */
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t sum_s2 = 0;
            for (idx_t i = 1; i < k; ++i) sum_s2 += r[i] * r[k - i];
            s2[k] = sum_s2;

            seq_t sum_a = 0;
            for (idx_t i = 0; i <= k; ++i) sum_a += s2[i] * s2[k - i];
            a_arr[k] = sum_a;

            /* r(k) = s2(k) + a(k) for n>1 */
            r[k] = sum_s2 + sum_a;
        }
        max_computed = n;
    }
    return a_arr[n];
}
