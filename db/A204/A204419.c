/*
 * A204419  y-values in the solutions to x^2 - 313*y^2 = 1.
 * https://oeis.org/A204419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204419: y-values in the solutions to x^2 − 313*y^2 = 1 */
/* Recurrence: a(1)=0, a(2)=1819380158564160,
   a(n) = 64376241658269698*a(n-1) − a(n-2)   for n ≥ 3 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* highest index already computed */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    /* Ensure base cases exist */
    if (cache_size == 0) {
        size_t init_cap = 2;
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        cache[0] = (seq_t)0ULL;                           /* a(1) */
        cache[1] = (seq_t)1819380158564160ULL;            /* a(2) */
        cache_size = 2;
    }

    /* Return if already computed */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Grow cache to size n (allow extra space) */
    size_t new_cap = n;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    const seq_t COEFF = (seq_t)64376241658269698ULL;

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t idx = i - 1;  /* array index for a(i) */
        cache[idx] = COEFF * cache[idx - 1] - cache[idx - 2];
    }

    cache_size = n;
    return cache[n - 1];
}
