/*
 * A147593  Expansion of 1/(1 - x + x^3 - 3*x^4 + x^5 - x^7 + x^8).
 * https://oeis.org/A147593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 71, u64 n <= 139,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A147593 – Expansion of 1/(1 - x + x^3 - 3*x^4 + x^5 - x^7 + x^8)
 *
 *  Recurrence (derived from the denominator polynomial):
 *    a(n) = a(n-1)          – a(n-3)
 *          + 3*a(n-4)        – a(n-5)
 *          +   a(n-7)        – a(n-8)        for n >= 8
 *
 *  Initial values:
 *    a(0)=1, a(1)=1, a(2)=1, a(3)=0,
 *    a(4)=2, a(5)=3, a(6)=5, a(7)=3
 *
 *  The function below memoises computed terms in a dynamic array so that
 *  calls can be made in arbitrary order and each term is calculated
 *  only once.
 */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

static seq_t *cache        = NULL;   /* array of computed terms   */
static size_t cache_used   = 0;      /* number of terms stored    */
static size_t cache_cap    = 0;      /* allocated capacity        */

/* Ensure the cache can hold at least 'need' slots (0‑based index). */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = need;
    if (new_cap < 16)
        new_cap = 16;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        return;   /* allocation failed – leave previous cache unchanged */

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Initialise the cache with the 8 known initial terms. */
static void init_cache(void)
{
    if (cache_cap != 0)
        return;

    ensure_capacity(8);
    if (cache_cap == 0)
        return;   /* out of memory */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)0;
    cache[4] = (seq_t)2;
    cache[5] = (seq_t)3;
    cache[6] = (seq_t)5;
    cache[7] = (seq_t)3;
    cache_used = 8;
}

/* Compute terms up to and including n (assumes n >= 0). */
static void compute_up_to(idx_t n)
{
    if ((idx_t)cache_used > n)
        return;

    /* Ensure enough space */
    size_t need = (size_t)n + 1;
    if (need > cache_cap)
        ensure_capacity(need);
    if (need > cache_cap)   /* still failed – cannot compute further */
        return;

    idx_t i;
    for (i = (idx_t)cache_used; i <= n; ++i)
    {
        seq_t val;
        if (i < 8)
        {
            val = cache[i];
        }
        else
        {
            /* a(i) = a(i-1) - a(i-3) + 3*a(i-4) - a(i-5) + a(i-7) - a(i-8) */
            val = cache[i - 1];
            val -= cache[i - 3];
            val += cache[i - 4] * (seq_t)3;
            val -= cache[i - 5];
            val += cache[i - 7];
            val -= cache[i - 8];
        }
        cache[i] = val;
    }
    cache_used = (size_t)n + 1;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    init_cache();

    if (cache_cap == 0)                /* allocation failed */
        return (seq_t)0;

    if ((size_t)n >= cache_used)
    {
        compute_up_to(n);
    }

    return cache[(size_t)n];
}
