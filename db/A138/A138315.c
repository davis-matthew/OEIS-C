/*
 * A138315  E.g.f. satisfies: A(x) = exp( Sum_{n>=0} a(n)*x^(2n+1)/(2n+1) ).
 * https://oeis.org/A138315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Recurrence derived from A(x) = exp( Σ_{n≥0} a(n)*x^(2n+1)/(2n+1) ) :

   a(0) = 1
   for n ≥ 0
       a(n+1) = Σ_{k=0}^{⌊n/2⌋} a(k) * a(n‑2k) * ( n! / (n‑2k)! )
*/

#include <stddef.h>
#include <stdlib.h>

/* Dynamic cache for computed values */
static seq_t *cache   = NULL; /* array of a(n) */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* last index computed */

/* Ensure cache[0..n] are computed and allocated */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                     /* memory error */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Set the initial value a(0) if not already done */
    if (cache_len == 0 && cache_cap > 0)
        cache[0] = 1;                     /* a(0) = 1 */

    /* Compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t N = i - 1;                   /* current recurrence index */
        idx_t max_k = N >> 1;              /* floor(N/2) */

        seq_t sum   = 0;
        seq_t ratio = 1;                   /* N!/(N-2k)! starts with 1 for k=0 */

        for (idx_t k = 0; k <= max_k; ++k) {
            if (k > 0) {
                seq_t p1 = (seq_t)(N - 2*k + 2);
                seq_t p2 = (seq_t)(N - 2*k + 1);
                ratio = ratio * p1 * p2;   /* multiply by (N-2k+2)(N-2k+1) */
            }
            seq_t term = cache[N - 2*k] * cache[k];
            term = term * ratio;
            sum = sum + term;
        }
        cache[i] = sum;
    }

    cache_len = n;
}

/* External function requested by the framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
