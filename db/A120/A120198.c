/*
 * A120198  a(1)=5; a(n)=floor((44+sum(a(1) to a(n-1)))/8).
 * https://oeis.org/A120198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5;
   a(n)=⌊(44+∑_{k=1}^{n-1}a(k))/8⌋   (for n≥2) */

/* This implementation keeps two static caches:
     - cache_a   : 0‑based array of a(n)
     - cache_cum : cumulative sums, cache_cum[i] = ∑_{k=1}^i a(k),
       with cache_cum[0] = 0.
   It expands the arrays lazily so that a(n) works for any n ≥ 1,
   regardless of call order. */

#include <stddef.h>
#include <stdlib.h>

/* static state */
static seq_t *cache_a     = NULL;   /* a(n) values, 0‑based */
static seq_t *cache_cum   = NULL;   /* cumulative sums, length = size+1 */
static idx_t   cache_size = 0;      /* number of terms currently stored */

/* ensure that cache arrays hold at least n terms */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* grow a(n) array to size n */
    cache_a   = (seq_t *)realloc(cache_a, (size_t)n   * sizeof(*cache_a));

    /* grow cumulative array to size n+1 (cum[0] = 0) */
    cache_cum = (seq_t *)realloc(cache_cum, (size_t)(n+1) * sizeof(*cache_cum));

    /* initialise cum[0] if this is the first allocation */
    if (cache_size == 0)
        cache_cum[0] = (seq_t)0;

    /* sum of all already computed terms */
    seq_t sum = cache_cum[cache_size];

    /* compute new terms and cumulative sums */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t new_term = ((seq_t)44 + sum) / (seq_t)8;  /* floor division */
        cache_a[i - 1] = new_term;
        sum += new_term;
        cache_cum[i] = sum;
    }
    cache_size = n;
}

/* Required function definition */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 harmlessly */

    ensure_cache(n);
    return cache_a[n - 1];
}
