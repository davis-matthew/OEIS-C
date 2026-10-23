/*
 * A042183  Denominators of continued fraction convergents to sqrt(616).
 * https://oeis.org/A042183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042183: Denominators of continued‑fraction convergents to sqrt(616).
    Recurrence:  a(n) = 42590*a(n-10) - a(n-20)   (n ≥ 20)
    Initial values (0 ≤ n ≤ 19) are given in the table below.  The
    function computes values on demand; intermediate results are kept
    in a cache that expands as needed.  The implementation uses only
    standard C functions and arithmetic operations allowed by the
    specification.  */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* pointer to cached terms               */
static size_t cache_size = 0;        /* current allocated array size          */
static size_t computed_up_to = 0;    /* highest index that has been filled    */

/* Initial values for n = 0 … 19 */
static const seq_t init_vals[20] = {
    (seq_t)1ULL,   (seq_t)1ULL,   (seq_t)5ULL,   (seq_t)6ULL,
    (seq_t)11ULL,  (seq_t)72ULL,  (seq_t)83ULL,  (seq_t)155ULL,
    (seq_t)703ULL, (seq_t)858ULL, (seq_t)41887ULL,(seq_t)42745ULL,
    (seq_t)212867ULL,(seq_t)255612ULL,(seq_t)468479ULL,(seq_t)3066486ULL,
    (seq_t)3534965ULL,(seq_t)6601451ULL,(seq_t)29940769ULL,(seq_t)36542220ULL
};

/* Ensure that the cache is large enough to hold terms up to index n. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* array must hold indices 0…n */

    if (cache == NULL) {                    /* first call: allocate and initialise */
        cache_size = 20;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)                 /* allocation failure: abort */
            exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
        computed_up_to = 19;
    } else if (needed > cache_size) {       /* enlarge the array if needed */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      needed * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = needed;
    }
}

/* Return a(n).  The function is safe to call in any order. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Return a cached value that has already been computed. */
    if ((idx_t)n <= (idx_t)computed_up_to)
        return cache[(size_t)n];

    /* Compute missing terms sequentially using the recurrence. */
    for (size_t i = computed_up_to + 1; i <= (size_t)n; ++i) {
        seq_t t10 = cache[i - 10];
        seq_t t20 = cache[i - 20];
        cache[i] = ((seq_t)42590) * t10 - t20;
    }
    computed_up_to = (size_t)n;
    return cache[(size_t)n];
}
