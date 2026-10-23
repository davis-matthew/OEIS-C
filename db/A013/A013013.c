/*
 * A013013  exp(sinh(x)+log(x+1))=1+2*x+3/2!*x^2+5/3!*x^3+13/4!*x^4+37/5!*x^5...
 * https://oeis.org/A013013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let f(0)=1 and for n≥1 * f(n) = Σ_{k odd, 1≤k≤n} C(n−1, k−1) ·
 *            f(n−k) * Then a(n) = f(n) + n · f(n−1) * (For n=0 a(0)=1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

extern seq_t a(idx_t n);

/* Static cache of f(n) values. */
static seq_t *f_cache = NULL;     /* array of f values */
static idx_t  f_capacity = 0;     /* allocated size of f_cache */
static idx_t  f_max     = 0;      /* maximum n computed */

/* Ensure cache holds values up to n (inclusive) */
static void
ensure_f(idx_t n)
{
    if (n < f_max)
        return;                 /* already computed */

    /* Grow cache if necessary */
    if (n + 1 > f_capacity) {
        seq_t *new_cache = alloc_seq_array(n + 1);
        /* Copy existing data */
        if (f_cache != NULL) {
            memcpy(new_cache, f_cache, f_capacity * sizeof(seq_t));
            free_seq_array(f_cache);
        }
        /* initialize new entries to zero */
        for (idx_t i = f_capacity; i <= n; ++i)
            new_cache[i] = 0;
        f_cache     = new_cache;
        f_capacity  = n + 1;
    }

    /* Compute missing f values */
    for (idx_t m = f_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t comb = 1;            /* C(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            if (k & 1) {           /* k is odd */
                sum += comb * f_cache[m - k];
            }
            /* Update comb for next k:
             * comb = C(m-1, k) from C(m-1, k-1) */
            seq_t temp = comb * (seq_t)(m - k);
            comb = temp / (seq_t)k;
        }
        f_cache[m] = sum;
    }
    f_max = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Initialize cache if necessary */
    if (f_cache == NULL)
        ensure_f(0);   /* creates f_cache[0] = 0 */

    /* Ensure f[0] = 1 */
    if (f_cache[0] == 0)
        f_cache[0] = 1;

    ensure_f(n);

    /* a(n) = f(n) + n * f(n-1) */
    return f_cache[n] + (seq_t)n * f_cache[n - 1];
}
