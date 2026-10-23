/*
 * A104722  Self-convolution of repeated Catalan numbers.
 * https://oeis.org/A104722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = Sum_{k=0..n} Catalan(floor(k/2)) * Catalan(floor((n-k)/2))
   The Catalan numbers are computed by the recurrence
       C_0 = 1
       C_{m+1} = C_m * 2*(2m+1)/(m+2)
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cat = NULL;   /* array of Catalan numbers, indexed by m */
static idx_t cat_size = 0;  /* number of Catalan numbers currently stored (max index+1) */

/* Ensure that Catalan numbers up to index m are computed and stored. */
static void ensure_cat(idx_t m)
{
    if (cat_size > m) return;        /* already enough */

    /* grow array if needed */
    idx_t new_size = cat_size ? cat_size * 2 : 1;
    while (new_size <= m) new_size <<= 1;
    cat = (seq_t *)realloc(cat, new_size * sizeof(seq_t));

    /* initialise first Catalan if empty */
    if (cat_size == 0) {
        cat[0] = (seq_t)1;
        cat_size = 1;
    }

    /* compute remaining Catalan numbers */
    for (idx_t i = cat_size; i <= m; ++i) {
        idx_t n = i - 1;                     /* we compute C_{i} from C_{i-1} = C_n */
        seq_t num = (seq_t)2 * (seq_t)(2*n + 1);   /* 2*(2n+1) */
        seq_t den = (seq_t)(i + 1);                 /* (n+2) = (i+1) */
        cat[i] = cat[i - 1] * num / den;
    }
    cat_size = m + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* b(k) = Catalan(floor(k/2)).  Need Catalan up to floor(n/2). */
    idx_t max_m = n >> 1;              /* floor(n/2) */
    ensure_cat(max_m);

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m1 = k >> 1;                 /* floor(k/2) */
        idx_t m2 = (n - k) >> 1;           /* floor((n-k)/2) */
        sum += cat[m1] * cat[m2];
    }
    return sum;
}
