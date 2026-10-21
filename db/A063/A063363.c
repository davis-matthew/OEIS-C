/*
 * A063363  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          90 ).
 * https://oeis.org/A063363
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Base values: a(2) =
 *            -1, a(3) = 53, a(4) = 102, a(5) = 157, a(6) = 212, a(7) = 259,
 *            a(8) = 314, a(9) = 367, a(10) = 416, a(11) = 469, a(12) = 526,
 *            a(13) = 573
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size of cache array   */
static idx_t  max_computed = 13;  /* highest n for which value is set */

static void init_cache(void)
{
    if (cache != NULL)
        return;                       /* already initialised */

    /* Allocate space for indices 0 … 13 (size 14) */
    cache_cap = 14;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache)
        return;                       /* allocation failure – caller will get 0 */

    /* Set the known base values */
    cache[2]  = -1;
    cache[3]  = 53;
    cache[4]  = 102;
    cache[5]  = 157;
    cache[6]  = 212;
    cache[7]  = 259;
    cache[8]  = 314;
    cache[9]  = 367;
    cache[10] = 416;
    cache[11] = 469;
    cache[12] = 526;
    cache[13] = 573;
}

static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;  /* on failure we keep the old allocation */
    cache = tmp;
    cache_cap = new_cap;
}

static void compute_elements(idx_t up_to)
{
    if (up_to <= max_computed)
        return;

    for (idx_t i = max_computed + 1; i <= up_to; ++i)
    {
        if (i <= 13)           /* values already set in init_cache() */
            continue;

        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }
    max_computed = up_to;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behaviour below offset is unspecified */

    init_cache();
    ensure_capacity(n);
    compute_elements(n);
    return cache[(size_t)n];
}
