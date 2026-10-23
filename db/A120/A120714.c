/*
 * A120714  Expansion of
 *          2*x^2*(7+14*x+4*x^2)/((1+x-x^2)*(1-2*x-10*x^2-6*x^3)).
 * https://oeis.org/A120714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 15*a(n-2) + 26*a(n-3) - 3*a(n-4) - 24*a(n-5) - 2*a(n-6) + 6*a(n-7)
 * with initial values (offset 1):
 * a(1) = 0,  a(2) = 14,  a(3) = 42,  a(4) = 232,  a(5) = 974,
 * a(6) = 4522, a(7) = 20180
 */

#include <stdlib.h>

/* cache of already‑computed terms */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of terms stored (highest index that has been filled) */
static size_t cache_cap   = 0;   /* current allocated capacity */

/* ensure capacity for 'needed' terms (needed = number of terms, 0‑based index +1) */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache   = tmp;
    cache_cap = new_cap;
}

/* Sequence A120714 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* by convention, accessed indices < 1 are ignored */

    size_t idx = (size_t)(n - 1);   /* array index for a(n) (0‑based) */
    if (idx < cache_len)
        return cache[idx];

    /* compute missing terms up to and including idx */
    ensure_cache(idx + 1);
    for (size_t i = cache_len; i <= idx; ++i)
    {
        seq_t val;
        if (i <= 6)   /* initial terms, i = 0 .. 6 correspond to n = 1 .. 7 */
        {
            switch (i)
            {
                case 0: val = 0;    break;    /* a(1) */
                case 1: val = 14;   break;    /* a(2) */
                case 2: val = 42;   break;    /* a(3) */
                case 3: val = 232;  break;    /* a(4) */
                case 4: val = 974;  break;    /* a(5) */
                case 5: val = 4522; break;    /* a(6) */
                case 6: val = 20180;break;    /* a(7) */
            }
        }
        else          /* recurrence for i >= 7 (i.e., n >= 8) */
        {
            val = 15 * cache[i-2]
                + 26 * cache[i-3]
                -  3 * cache[i-4]
                - 24 * cache[i-5]
                -  2 * cache[i-6]
                +  6 * cache[i-7];
        }
        cache[i] = val;
    }
    cache_len = idx + 1;
    return cache[idx];
}
