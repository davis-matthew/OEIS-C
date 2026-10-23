/*
 * A167929  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n≥1: a(n)=18*a(n-1) – (n≥16?170*a(n-16):0) +
 *            (n≥17?153*a(n-17):0) + b(n) where b(0)=1, b(1)=1, b(16)=–1,
 *            b(17)=–1, otherwise b(n)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Initial terms (n = 0 … 15) are given explicitly. */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_terms[16] =
{
    1ULL,
    19ULL,
    342ULL,
    6156ULL,
    110808ULL,
    1994544ULL,
    35901792ULL,
    646232256ULL,
    11632180608ULL,
    209379250944ULL,
    3768826516992ULL,
    67838877305856ULL,
    1221099791505408ULL,
    21979796247097344ULL,
    395636332447752192ULL,
    7121453984059539456ULL
};

static seq_t *cache = NULL;          /* cache for computed terms */
static idx_t   cache_cap = 0;         /* capacity of cache */
static idx_t   last_computed = 0;     /* highest index stored in cache */

/* ensure cache can hold indices up to n inclusive */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? (size_t)cache_cap : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)   /* allocation failed; leave cache unchanged */
            return;
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }
}

/* initialise cache[0..15] with known values; other positions left uninitialised */
static void init_cache_if_needed(void)
{
    if (!cache) {                      /* first use */
        size_t init_cap = 16;           /* store 0..15 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            return;
        cache_cap = (idx_t)init_cap;
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_terms[i];
        last_computed = 15;
    }
}

/* compute terms up to n, using the recurrence above */
seq_t a(idx_t n)
{
    if (n < 16)
        return init_terms[n];

    init_cache_if_needed();           /* guarantees cache[0..15] exist */

    /* Extend cache if necessary */
    if (n > last_computed)
        ensure_cache(n);

    /* recurrence constants cast to seq_t */
    const seq_t c18  = (seq_t)18;
    const seq_t c170 = (seq_t)170;
    const seq_t c153 = (seq_t)153;

    /* recurrence for i from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * c18;
        if (i >= 16)
            term -= cache[i - 16] * c170;
        if (i >= 17)
            term += cache[i - 17] * c153;

        /* b(i) contribution */
        if (i == 0 || i == 1)
            term += 1;
        else if (i == 16 || i == 17)
            term -= 1;

        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
