/*
 * A086611  Row sums of triangle of coefficients (A086610) where T(n,k) is the
 *          coefficient of x^n*y^k in f(x,y) that satisfies f(x,y) =
 *          (1-x-x^2)/(1-x)^2 + xy*f(x,y)^2.
 * https://oeis.org/A086611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1, a(n)=2‑n+Σ_{k=0}^{n‑1} a(k)·a(n‑1‑k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are provided by the build system */

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static idx_t cache_len = 0;      /* number of terms currently cached (== cache_len == max_index+1) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already computed */

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;              /* indices 0..n inclusive */

    /* Allocate or grow the array.  realloc(NULL, sz) behaves like malloc. */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) abort();           /* allocation failure */
    cache = tmp;
    cache_len = new_len;

    if (old_len == 0)                  /* no terms computed yet */
        cache[0] = (seq_t)1;           /* a(0)=1 */

    /* Compute all terms from the first missing index up to n */
    idx_t start = (old_len == 0) ? 1 : old_len;   /* recurrence starts at n=1 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = (seq_t)2 - (seq_t)i;          /* 2‑i part of the recurrence */
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
