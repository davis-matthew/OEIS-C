/*
 * A025769  Expansion of 1/((1-x)*(1-x^3)*(1-x^8)).
 * https://oeis.org/A025769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{⌊n/8⌋} (⌊(n-8k)/3⌋ + 1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=1, a(1)=1, a(2)=1: the formula yields these as well. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values */
static idx_t cache_size = 0;  /* number of values currently cached */

/* Allocate/extend the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;  /* already computed */

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(1);                  /* allocation failure */

    cache = tmp;

    /* Compute values from the previous cache size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i / 8; ++k) {
            idx_t t = i - 8 * k;               /* remaining after k*8 */
            seq_t part = (seq_t)(t / 3) + 1;   /* floor((t)/3)+1  */
            sum += part;
        }
        cache[i] = sum;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
