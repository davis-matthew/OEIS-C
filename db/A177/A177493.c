/*
 * A177493  Products of cubes of 2 or more distinct primes.
 * https://oeis.org/A177493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A120944(n))³
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 734, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A120944(n) is the nth product of distinct primes (square‑free composite numbers) */

#include <stdlib.h>

/* Static state for caching computed terms and the smallest prime factor table */
static idx_t *spf = NULL;                 /* smallest prime factor for each integer */
static idx_t  spf_limit = 0;              /* maximum integer for which spf is valid */

static seq_t  *cache = NULL;              /* array of computed a(n) values */
static size_t cache_cap  = 0;             /* allocated capacity of cache   */
static size_t cache_size = 0;             /* number of values stored so far */

static idx_t next_test = 2;               /* next integer to test for inclusion */

/* Ensure the spf array covers at least all integers up to 'limit'.
   The array is resized if necessary. */
static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit) return;

    /* Allocate a new array and build the sieve */
    idx_t *new_spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!new_spf) exit(1);                  /* allocation failure */

    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    if (limit >= 1) new_spf[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {              /* i is prime */
            new_spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == 0) new_spf[j] = i;
        }
    }

    /* Replace the old array (if any) with the new one */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Grow the cache array to accommodate at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);               /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return 1 if n is square‑free, 0 otherwise.  The spf array must already
   cover 'n'. */
static int is_squarefree(idx_t n)
{
    idx_t temp   = n;
    idx_t last_p = 0;
    while (temp > 1) {
        idx_t p = spf[temp];
        temp   /= p;
        if (p == last_p) return 0;          /* repeated prime factor */
        last_p = p;
    }
    return 1;
}

/* Main sequence function requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n < 1, safety */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size) return cache[n-1];

    /* Generate terms until the requested index is reached */
    while (cache_size < (size_t)n) {
        /* Grow the spf table sufficiently to test 'next_test' */
        if ((idx_t)(next_test + 1) > spf_limit) {
            idx_t new_limit = (idx_t)(next_test * 2 + 1000);
            ensure_spf_limit(new_limit);
        }

        if (next_test <= 1) {        /* skip 0 and 1 */
            ++next_test;
            continue;
        }

        /* Test whether 'next_test' is a composite square‑free integer
           (i.e., a product of at least two distinct primes). */
        if (spf[next_test] != next_test && is_squarefree(next_test)) {
            /* Compute the cube of the product of distinct primes */
            seq_t base  = (seq_t)next_test;
            seq_t cube  = base * base * base;   /* works for both fixed-width and big ints */

            /* Store in cache */
            ensure_cache_size(cache_size + 1);
            cache[cache_size++] = cube;
        }

        ++next_test;
    }

    return cache[n-1];
}
