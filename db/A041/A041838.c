/*
 * A041838  Numerators of continued fraction convergents to sqrt(440).
 * https://oeis.org/A041838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 42*a(n-2) - a(n-4); a(0)=20, a(1)=21, a(2)=860, a(3)=881
 *            (OEIS A041838 – numerators of continued‑fraction convergents to
 *            sqrt(440))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array of computed terms */
static size_t cache_cap = 0;          /* allocated capacity of cache */
static idx_t  cache_max = (idx_t)-1;  /* highest index for which cache[i] is valid */

static const seq_t init_vals[4] = {20, 21, 860, 881};

/* Ensure that cache contains terms up to and including index n. */
static void ensure_cache_up_to(idx_t n)
{
    /* First allocation: initialise the first four terms. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        cache_max = 3;
    }

    if (n <= cache_max) return;          /* already computed */

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) { /* allocation failure – terminate */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values iteratively. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i >= 4) {
            cache[i] = 42 * cache[i - 2] - cache[i - 4];
        } else { /* i < 4 – safety copy initial values */
            cache[i] = init_vals[i];
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache_up_to(n);
    return cache[n];
}
