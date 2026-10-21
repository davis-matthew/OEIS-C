/*
 * A231139  Number of (n+1) X (2+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal and vertical neighbors, with
 *          upper left element zero.
 * https://oeis.org/A231139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            14*a(n-1)+28*a(n-2)-83*a(n-3)-92*a(n-4)-342*a(n-5)+134*a(n-6)+890*a(n-7)+432*a(n-8)-32*a(n-9)+80*a(n-10)-320*a(n-11)
 *            Empirical linear recurrence. Initial values (a(1) … a(11)): 100,
 *            1470, 22870, 352444, 5441634, 83985430, 1296319964, 20008457170,
 *            308827818618, 4766712219328, 73573516286674
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* array of cached terms, 1‑based */
static idx_t  cache_capacity = 0;  /* bytes allocated = cache_capacity*sizeof(seq_t) */
static idx_t  max_computed   = 0;   /* highest n whose value is stored */

/* Allocate space for indices 0 .. n (index 0 unused). */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_capacity) return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);      /* allocation failed */

    cache      = new_cache;
    cache_capacity = new_cap;
}

/* Initial terms (a(1) … a(11)).  Index 0 is unused. */
static const seq_t base[12] = {
    (seq_t)0ULL,                     /* unused */
    (seq_t)100ULL,                   /* a(1) */
    (seq_t)1470ULL,                  /* a(2) */
    (seq_t)22870ULL,                 /* a(3) */
    (seq_t)352444ULL,                /* a(4) */
    (seq_t)5441634ULL,               /* a(5) */
    (seq_t)83985430ULL,              /* a(6) */
    (seq_t)1296319964ULL,            /* a(7) */
    (seq_t)20008457170ULL,           /* a(8) */
    (seq_t)308827818618ULL,          /* a(9) */
    (seq_t)4766712219328ULL,         /* a(10) */
    (seq_t)73573516286674ULL         /* a(11) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                      /* no term for n = 0 */

    if (n <= 11) return base[n];                      /* return pre‑computed term */

    /* Make sure the cache can hold indices up to n. */
    ensure_cache(n);

    /* Compute all missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 11) {                                 /* copy initial terms */
            cache[i] = base[i];
        } else {
            /* Use seq_t arithmetic to avoid overflow. */
            seq_t pos = cache[i-1] * (seq_t)14ULL
                       + cache[i-2] * (seq_t)28ULL
                       + cache[i-6] * (seq_t)134ULL
                       + cache[i-7] * (seq_t)890ULL
                       + cache[i-8] * (seq_t)432ULL
                       + cache[i-10] * (seq_t)80ULL;

            seq_t neg = cache[i-3] * (seq_t)83ULL
                       + cache[i-4] * (seq_t)92ULL
                       + cache[i-5] * (seq_t)342ULL
                       + cache[i-9] * (seq_t)32ULL
                       + cache[i-11] * (seq_t)320ULL;

            cache[i] = pos - neg;
        }
    }

    max_computed = n;
    return cache[n];
}
