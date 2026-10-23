/*
 * A009542  Expansion of e.g.f. sin(x)*exp(sinh(x)).
 * https://oeis.org/A009542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n/n! in sin(x)*exp(sinh(x))
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 28, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the recurrence for the egf of exp(sinh(x)) */
/*   b(0) = 1,   b(n) = Σ_{k odd≤n} b(n-k) * C(n-1,k-1)    */
/* and a(n) = Σ_{i odd≤n} (-1)^((i-1)/2) * C(n,i) * b(n-i) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for the auxiliary sequence b(n) */
static seq_t *b_cache = NULL;    /* array of b values */
static idx_t  b_max   = 0;       /* highest n computed and stored in b_cache */

/* Ensure that b_cache has entries up to n (inclusive) */
static void ensure_b(idx_t n)
{
    if (n <= b_max) return;                 /* already computed */

    /* grow the array */
    idx_t new_capacity = (b_max == 0) ? 1 : b_max * 2;
    while (new_capacity < n + 1) new_capacity <<= 1;

    seq_t *new_cache = (seq_t *)realloc(b_cache, new_capacity * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);     /* allocation failed */
    b_cache = new_cache;

    /* initialise new entries */
    if (b_max == 0) {
        b_cache[0] = (seq_t)1;
        b_max = 0;
    }

    /* compute b values from b_max+1 up to n */
    for (idx_t m = b_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1; /* C(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            if (k >= 2) {
                /* update bin to C(m-1,k-1) */
                bin = bin * (seq_t)(m - k + 1) / (seq_t)(k - 1);
            }
            if ((k & 1) == 1) {            /* k odd */
                sum += b_cache[m - k] * bin;
            }
        }
        b_cache[m] = sum;
    }
    b_max = n;
}

/* Main function required by the platform */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    /* compute auxiliary sequence up to n */
    ensure_b(n);

    /* compute a(n) */
    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1; /* C(n,0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* update bin to C(n,i) */
        bin = bin * (seq_t)(n - i + 1) / (seq_t)i;

        if ((i & 1) == 1) {              /* i odd */
            int sign = (((i - 1) >> 1) & 1) ? -1 : 1;
            if (sign > 0)
                sum += bin * b_cache[n - i];
            else
                sum -= bin * b_cache[n - i];
        }
    }
    return sum;
}
