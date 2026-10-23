/*
 * A019308  Number of "bifix-free" words of length n over a three-letter
 *          alphabet.
 * https://oeis.org/A019308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n+1) = 3·a(2n) a(2n) = 3·a(2n‑1) – a(n) a(0) = 1, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Persistent cache of computed values */
static seq_t *cache     = NULL;      /* array of seq_t values              */
static size_t cap       = 0;         /* current allocated capacity         */
static idx_t  max_comp  = 0;         /* highest index whose value is stored */

/* Compute a(n) as defined by the recurrence. */
seq_t a(idx_t n)
{
    /* --- initialise the cache with the base values ------------------------------------ */
    if (cache == NULL) {
        size_t init_cap = 2;                      /* for indices 0 and 1          */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (!cache) abort();
        cap = init_cap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        max_comp = 1;
    }

    /* --- if the requested value is already known ------------------------------------- */
    if (n <= max_comp)
        return cache[n];

    /* --- grow the cache enough to hold the requested index ---------------------------- */
    size_t needed = (size_t)(n + 1);             /* number of elements needed    */
    if (needed > cap) {
        size_t new_cap = cap ? (cap << 1) : 2;    /* power‑of‑two growth          */
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* --- compute missing values up to n using the recurrence -------------------------- */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if ((i & 1U) != 0) {          /* odd index: a(2k+1) = 3 * a(2k)    */
            cache[i] = cache[i - 1] * (seq_t)3;
        } else {                      /* even index: a(2k) = 3 * a(2k-1) – a(k) */
            cache[i] = cache[i - 1] * (seq_t)3 - cache[i / 2];
        }
    }
    max_comp = n;
    return cache[n];
}
