/*
 * A170576  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence from G.f. * Q(t) = 1 - 37 t - 37 t^2 - … - 37 t^46 +
 *            703 t^47 * P(t) = 1 + 2 t + … + 2 t^46 + t^47 * * For n ≥ 0 *
 *            a(n) = 37 · Σ_{k=1}^{min(n,46)} a(n-k) * − 703 · a(n-47) (term
 *            zero for n < 47) * + r(n) , * where r(n) = 1 for n = 0,47 * 2
 *            for 1 ≤ n ≤ 46 * 0 otherwise. * * The implemen...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* cached terms */
static seq_t *cache   = NULL;   /* array of computed values          */
static size_t cache_cap = 0;    /* allocation size (power of two)    */
static idx_t  cache_len = 0;    /* number of terms that are populated*/

/* Ensure that cache holds at least indices 0 … up_to inclusive. */
static void
ensure_cache_size(idx_t up_to)
{
    if (cache_cap > (size_t)up_to) return;            /* already large enough */

    size_t new_cap = cache_cap ? cache_cap : 64;     /* start with 64 or double */
    while (new_cap <= (size_t)up_to) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                         /* allocation failure – unlikely */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return a(n) for the Coxeter group sequence. */
seq_t
a(idx_t n)
{
    ensure_cache_size(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;

        /* 37 · Σ a(i-k) for k = 1 … min(i,46) */
        for (idx_t k = 1; k <= 46 && k <= i; ++k) {
            val += cache[i - k] * 37;
        }

        /* subtract 703·a(i-47) if i≥47 (otherwise a(i-47)=0) */
        if (i >= 47) {
            val -= cache[i - 47] * 703;
        }

        /* add the numerator coefficient r(i) */
        if (i <= 47) {
            if (i == 0 || i == 47) {
                val += 1;
            } else {
                val += 2;
            }
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
