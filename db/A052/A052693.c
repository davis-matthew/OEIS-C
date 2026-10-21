/*
 * A052693  Expansion of e.g.f. (1-x)/(1-3*x+x^3).
 * https://oeis.org/A052693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052693
   Recurrence:  a(0)=1, a(1)=2, a(2)=12
   For n ≥ 0:  a(n+3) = 3*(n+3)*a(n+2) – (n+1)*(n+2)*(n+3)*a(n);
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_allocated = 0;   /* size of allocated block */
static idx_t cache_valid = 0;       /* number of terms currently stored */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    idx_t need = n + 1;
    if (need <= cache_allocated)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    cache = tmp;
    cache_allocated = need;
}

/* Compute a(m) using the recurrence, assuming cache[m‑1] and cache[m‑3] are set */
static seq_t compute_term(idx_t m)
{
    /* a(m) = 3*m*a(m‑1) – (m‑2)*(m‑1)*m*a(m‑3) */
    seq_t t1 = ((seq_t)3 * (seq_t)m) * cache[m - 1];
    seq_t t2 = ((seq_t)(m - 2) * (seq_t)(m - 1) * (seq_t)m) * cache[m - 3];
    return t1 - t2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;

    ensure_cache(n);

    /* Initialise the first three terms if the cache is empty */
    if (cache_valid == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        cache_valid = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t m = cache_valid; m <= n; ++m) {
        cache[m] = compute_term(m);
    }
    cache_valid = n + 1;   /* all terms up to n are now stored */

    return cache[n];
}
