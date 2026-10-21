/*
 * A063361  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          88 ).
 * https://oeis.org/A063361
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) + a(n-4) - a(n-6) for n>8 initial values: a(2) =
 *            -1, a(3) = 115, a(4) = 248, a(5) = 379, a(6) = 514, a(7) = 645,
 *            a(8) = 780
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of calculated values */
static size_t  cap   = 0;        /* allocated size of cache */
static size_t  len   = 0;        /* first index beyond last initialized value */

/* Ensure array capacity for indices up to need (inclusive) */
static void ensure_capacity(size_t need)
{
    if (need < cap) return;
    size_t new_cap = cap ? cap : 64;
    while (new_cap <= need) new_cap = new_cap * 2 + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);               /* allocation failure */
    for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0; /* initialise new slots */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise base values for indices 2..8 */
static void init_cache(void)
{
    if (cache) return;
    cache = (seq_t *)malloc(64 * sizeof(seq_t));
    if (!cache) exit(1);
    for (size_t i = 0; i < 64; ++i) cache[i] = 0;
    cap = 64;
    len = 9; /* indices 0..8 are now defined */
    cache[2] = -1;       /* a(2) */
    cache[3] = 115;      /* a(3) */
    cache[4] = 248;      /* a(4) */
    cache[5] = 379;      /* a(5) */
    cache[6] = 514;      /* a(6) */
    cache[7] = 645;      /* a(7) */
    cache[8] = 780;      /* a(8) */
}

/* Compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is arbitrary */

    size_t idx = (size_t)n;       /* indices up to 10000 fit in size_t */
    init_cache();

    /* Extend cache up to n if necessary */
    if (len <= idx)
    {
        ensure_capacity(idx);

        /* Compute missing terms using the recurrence */
        for (size_t i = len; i <= idx; ++i)
        {
            /* For i > 8, recurrence holds. For i = 9..8? already set. */
            cache[i] = cache[i - 2] + cache[i - 4] - cache[i - 6];
        }
        len = idx + 1; /* next uninitialised index */
    }
    return cache[idx];
}
