/*
 * A171205  G.f. satisfies: A(x) = (1 + x*A(2x))^5.
 * https://oeis.org/A171205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = (1 + x*A(2x))^5 (OEIS A171205)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* binomial coefficients for (1 + y)^5 */
static const int choose[6] = {1, 5, 10, 10, 5, 1};

/* cached data */
static idx_t  computed_max = 0;            /* largest index computed   */
static seq_t *a_arr        = NULL;          /* a[n]                     */
static seq_t *pow2_arr     = NULL;          /* 2^n                      */
static seq_t *conv[6]      = {NULL, NULL, NULL, NULL, NULL, NULL}; /* conv[k][n] */

/* extend all arrays to include indices up to new_max (inclusive) */
static void extend(idx_t new_max) {
    idx_t old_max = computed_max;

    /* allocate / reallocate arrays */
    a_arr    = (seq_t*)realloc(a_arr,    (new_max + 1) * sizeof(seq_t));
    pow2_arr = (seq_t*)realloc(pow2_arr, (new_max + 1) * sizeof(seq_t));
    for (int k = 1; k <= 5; ++k)
        conv[k] = (seq_t*)realloc(conv[k], (new_max + 1) * sizeof(seq_t));

    /* zero new entries (except for base case) */
    if (old_max < new_max) {
        memset(a_arr + old_max + 1,    0, (new_max - old_max) * sizeof(seq_t));
        memset(pow2_arr + old_max + 1, 0, (new_max - old_max) * sizeof(seq_t));
        for (int k = 1; k <= 5; ++k)
            memset(conv[k] + old_max + 1, 0, (new_max - old_max) * sizeof(seq_t));
    }

    /* initialise base case (already done only once) */
    if (old_max == 0) {
        a_arr[0] = (seq_t)1;
        pow2_arr[0] = (seq_t)1;
        for (int k = 1; k <= 5; ++k)
            conv[k][0] = (seq_t)1;          /* a0^k = 1 */
    }

    /* compute missing 2^n values */
    for (idx_t i = old_max + 1; i <= new_max; ++i)
        pow2_arr[i] = pow2_arr[i - 1] * (seq_t)2;

    /* compute sequence values and convolution tables from old_max+1 up to new_max */
    for (idx_t n = old_max + 1; n <= new_max; ++n) {

        /* compute a[n] using the functional equation */
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 5; ++k) {
            if (n < (idx_t)k) break;                  /* term would involve negative index */
            seq_t term = conv[k][n - k];
            term *= (seq_t)choose[k];                  /* multiply by binomial coefficient */
            term *= pow2_arr[n - k];                   /* multiply by 2^(n-k) */
            sum += term;
        }
        a_arr[n] = sum;

        /* update convolution tables conv[1..5] for index n */
        conv[1][n] = a_arr[n];                         /* trivial: a[n] */

        for (int k = 2; k <= 5; ++k) {
            seq_t acc = (seq_t)0;
            /* conv[k][n] = sum_{i=0..n} a[i] * conv[k-1][n-i] */
            for (idx_t i = 0; i <= n; ++i)
                acc += a_arr[i] * conv[k - 1][n - i];
            conv[k][n] = acc;
        }
    }

    computed_max = new_max;
}

/* required external function */
seq_t a(idx_t n) {
    if (a_arr == NULL) {
        /* first call – initialise for index 0 at least */
        extend(0);
    }
    if (n > computed_max)
        extend(n);
    return a_arr[n];
}
