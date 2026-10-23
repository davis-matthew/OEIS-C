/*
 * A041566  Numerators of continued fraction convergents to sqrt(301).
 * https://oeis.org/A041566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041566: Numerators of convergents to sqrt(301)
   Recurrence: let p_-1 = 1, p_0 = 17.
   For n >= 1, p_n = c_n * p_{n-1} + p_{n-2},
   where c_1, c_2, … are the periodic partial quotients of the
   continued fraction of sqrt(301) (c_1 = 2, c_2 = 4, …).
   Then a(n) = p_n.
   Initial values: a(0)=17, a(1)=35, a(2)=52, … 
*/

#include <stdlib.h>
#include <stdint.h>

static idx_t period_len = 0;
static idx_t period[512] = {0};

static void init_period(void)
{
    if (period_len != 0) return;          /* already initialized */

    /* floor(sqrt(301)) = 17 */
    const idx_t N   = 301;
    const idx_t a0  = 17;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    do
    {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    } while (d != 1);   /* period ends when d == 1 for quadratic surds */
}

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* allocated capacity */

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    /* enlarge storage if necessary */
    if (cache_size == 0)
    {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    else
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first two terms if we are starting from scratch */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)17;   /* p_0 */
        init_period();
        cache[1] = (seq_t)period[0] * cache[0] + (seq_t)1;  /* p_1 */
        cache_size = 2;
    }

    /* extend cache up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* coefficient c_i for step i (i >= 2) is period[(i-1)%period_len] */
        idx_t coeff_idx = (i - 1) % period_len;
        seq_t coeff     = (seq_t)period[coeff_idx];
        cache[i] = coeff * cache[i - 1] + cache[i - 2];
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)17;
    ensure_cache(n);
    return cache[n];
}
