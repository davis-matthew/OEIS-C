/*
 * A201969  G.f. satisfies: A(x) = 1 + x*A(x)^2 + x^2/A(x).
 * https://oeis.org/A201969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from functional equation A(x) = 1 + x*A(x)^2 + x^2/A(x);
   computes coefficients a(n) iteratively using convolution. */
#include <stdlib.h>
#include <stdint.h>

/* Static storage for dynamic arrays */
static idx_t max_n = 0;
static seq_t *a_arr = NULL, *c_arr = NULL, *d_arr = NULL;
static size_t arr_cap = 0;

/* Resize arrays if needed */
static void ensure_capacity(idx_t n) {
    if (arr_cap >= (size_t)(n + 1))
        return;
    size_t new_cap = arr_cap ? arr_cap * 2 : 64;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;
    seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_c = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_d = (seq_t*)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < arr_cap; ++i) {
        new_a[i] = a_arr[i];
        new_c[i] = c_arr[i];
        new_d[i] = d_arr[i];
    }
    free(a_arr);
    free(c_arr);
    free(d_arr);
    a_arr = new_a;
    c_arr = new_c;
    d_arr = new_d;
    arr_cap = new_cap;
}

/* Initialise base case values (a(0) … a(1)) */
static void init_base(void) {
    ensure_capacity(1);
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)1;
    c_arr[0] = (seq_t)1;          /* a0*a0 */
    c_arr[1] = (seq_t)2;          /* 2*a0*a1 */
    d_arr[0] = (seq_t)1;          /* a0*a0*a0 */
    d_arr[1] = (seq_t)3;          /* a0*c1 + a1*c0 = 1*2 + 1*1 */
    max_n   = 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (max_n == 0)
        init_base();
    if ((idx_t)n > max_n) {
        ensure_capacity((idx_t)n);
        for (idx_t k = max_n + 1; k <= (idx_t)n; ++k) {
            /* s_k = Σ_{i=1}^{k-1} a[i] * a[k-i] */
            seq_t s = 0;
            for (idx_t i = 1; i < k; ++i)
                s += a_arr[i] * a_arr[k - i];

            /* d_{k-1} = Σ_{i=0}^{k-1} a[i] * c[k-1-i] */
            seq_t d_prev = 0;
            for (idx_t i = 0; i < k; ++i)
                d_prev += a_arr[i] * c_arr[k - 1 - i];

            /* a(k) = d_{k-1} + [k==2] - s_k */
            seq_t val = d_prev + ((k == 2) ? (seq_t)1 : (seq_t)0);
            val -= s;
            a_arr[k] = val;

            /* c_k = 2*a_k + s_k */
            c_arr[k] = (seq_t)2 * a_arr[k] + s;

            /* d_k = Σ_{i=0}^k a[i] * c[k-i] */
            seq_t d_k = 0;
            for (idx_t i = 0; i <= k; ++i)
                d_k += a_arr[i] * c_arr[k - i];
            d_arr[k] = d_k;
        }
        max_n = (idx_t)n;
    }
    return a_arr[(idx_t)n];
}
