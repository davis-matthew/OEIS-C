/*
 * A084902  a(n) = 5^(n-1)*n*(n+1)/2.
 * https://oeis.org/A084902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^(n-1)*n*(n+1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of powers of 5 */
static seq_t *pow5 = NULL;   /* pow5[k] = 5^k, 0 <= k < pow5_len */
static idx_t pow5_len = 0;   /* number of precomputed entries */
static idx_t pow5_cap = 0;   /* allocated capacity */

/* Ensure that pow5[n] is valid for all n <= exp */
static void ensure_power5(idx_t exp) {
    if (pow5_len == 0) {                 /* initialisation */
        pow5_cap = 1;
        pow5 = (seq_t*)malloc(pow5_cap * sizeof(seq_t));
        pow5[0] = (seq_t)1;
        pow5_len = 1;
    }
    if (exp < pow5_len) return;          /* already computed */

    /* grow capacity to at least exp+1 */
    while (pow5_cap <= exp) pow5_cap <<= 1;
    pow5 = (seq_t*)realloc(pow5, pow5_cap * sizeof(seq_t));

    /* compute new entries */
    idx_t i;
    for (i = pow5_len; i <= exp; ++i) {
        pow5[i] = pow5[i - 1] * (seq_t)5;
    }
    pow5_len = exp + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;             /* offset is 0 */

    ensure_power5(n - 1);                     /* need 5^(n-1) */
    seq_t pow_val = pow5[n - 1];

    /* triangular number: n*(n+1)/2, converting n first */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    return pow_val * tri;
}
