/*
 * A089904  Sum of digits of numbers between 0 and (2/9)*(10^n-1).
 * https://oeis.org/A089904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (126*n + 77 + 10^n*(81*n - 77)) / 81 * (equivalent to the
 *            OEIS formula a(n) = (-77*(-1+10^n)+9*(14+9*10^n)*n)/81)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow10_cache = NULL;   /* pow10_cache[i] = 10^i */
static idx_t  pow10_cache_size = 0;

/* Compute 10^n, caching results to avoid recomputation. */
static seq_t get_pow10(idx_t n)
{
    if (pow10_cache_size == 0) {            /* initialise for n==0 */
        pow10_cache = (seq_t *)malloc(sizeof(seq_t));
        pow10_cache[0] = (seq_t)1;
        pow10_cache_size = 1;
    }
    if (n >= pow10_cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(pow10_cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failure will be ignored; value will be unreproducible */
            return (seq_t)1;
        pow10_cache = tmp;
        for (idx_t i = pow10_cache_size; i <= n; ++i) {
            pow10_cache[i] = pow10_cache[i - 1] * (seq_t)10;
        }
        pow10_cache_size = new_size;
    }
    return pow10_cache[n];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t pow10n = get_pow10(n);

    /* term_a = 126*n + 77 */
    seq_t term_a = (seq_t)126 * n + (seq_t)77;

    /* term_b = 10^n * (81*n - 77)  (81*n - 77 > 0 for n>=1) */
    seq_t term_b_coef = (seq_t)81 * n - (seq_t)77;
    seq_t term_b = pow10n * term_b_coef;

    seq_t numerator = term_a + term_b;
    seq_t result = numerator / (seq_t)81;
    return result;
}
