/*
 * A170225  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22 a(n-1)+…+22 a(n-39)-253 a(n-40)+c(n), where c(0)=1,
 *            c(1…39)=2, c(40)=1, c(n)=0 for n>40. Initial value a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *a_cache = NULL;      /* cached terms */
static idx_t  a_cache_size = 0;    /* number of cached terms */

static void
ensure_a(idx_t n)
{
    if (n < a_cache_size) return;

    /* Allocate or grow the cache to hold indices 0 … n */
    size_t new_cap = (size_t)(n + 1);
    if (a_cache_size == 0) {
        a_cache = seq_alloc(new_cap);
    } else {
        a_cache = seq_realloc(a_cache, a_cache_size, new_cap);
    }

    /* Compute the missing terms */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* 22 * (a[i-1] + … + a[i-39])  */
        for (idx_t k = 1; k <= 39 && i >= k; ++k)
            sum += a_cache[i - k] * (seq_t)22;

        /* -253 * a[i-40] if defined */
        if (i >= 40)
            sum -= a_cache[i - 40] * (seq_t)253;

        /* add the numerator coefficient */
        seq_t coeff;
        if (i == 0 || i == 40)
            coeff = (seq_t)1;
        else if (i <= 39)
            coeff = (seq_t)2;
        else
            coeff = (seq_t)0;

        a_cache[i] = sum + coeff;
    }

    a_cache_size = new_cap;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
