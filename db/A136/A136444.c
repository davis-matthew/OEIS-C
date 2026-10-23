/*
 * A136444  a(n) = Sum_{k=0..n} k*binomial(n-k, 2*k).
 * https://oeis.org/A136444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k*binomial(n-k, 2*k) Recurrence (derived
 *            from the generating function): a(n) = 4*a(n-1) - 6*a(n-2) +
 *            6*a(n-3) - 5*a(n-4) + 2*a(n-5) - a(n-6), for n >= 6. Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=6
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 75, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;           /* array of computed a(n) values      */
static idx_t  cache_len = 0;          /* number of elements currently cached (index+1) */

static const seq_t base[6] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)3, (seq_t)6 };

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                /* already computed */
        return;

    /* Allocate or reallocate the cache */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* memory allocation failed */

    cache = new_cache;

    /* Compute missing entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6) {
            cache[i] = base[(size_t)i];
        } else {
            /* Compute using the recurrence:
               a(n) = 4*a(n-1) + 6*a(n-3) + 2*a(n-5)
                      - (6*a(n-2) + 5*a(n-4) + a(n-6))  */
            seq_t pos = cache[i-1] * (seq_t)4
                      + cache[i-3] * (seq_t)6
                      + cache[i-5] * (seq_t)2;
            seq_t neg = cache[i-2] * (seq_t)6
                      + cache[i-4] * (seq_t)5
                      + cache[i-6];
            cache[i] = pos - neg;      /* guaranteed non‑negative */
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
