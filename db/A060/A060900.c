/*
 * A060900  Number of walks of length n on square lattice, starting at origin,
 *          staying on points with x >= 0, y <= x.
 * https://oeis.org/A060900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(2n) = a(2n-1) * (12n + 2) / (3n + 1) a(2n+1) = a(2n)
 *            * (4n + 2) / (n + 1) The division is always integral
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* previously computed terms */
static idx_t   cache_len = 0;    /* number of terms stored (index+1) */
static idx_t   cache_cap = 0;    /* array capacity */

/* Ensure the cache array has space for indices up to `needed`. */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!cache) abort();          /* memory allocation failed */
    cache_cap = new_cap;
}

/* Compute a(k) for all k from cache_len up to n inclusively. */
static void compute_up_to(idx_t n)
{
    if (n < cache_len) return;    /* already computed */

    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
        {
            val = (seq_t)1;
        }
        else if ((i & 1) == 0)          /* i = 2n */
        {
            idx_t k  = i >> 1;          /* n in the recurrence */
            seq_t num = (seq_t)(12 * k + 2);
            seq_t den = (seq_t)(3 * k + 1);
            /* Use 128‑bit intermediate when seq_t is 64‑bit or smaller. */
            if (sizeof(seq_t) <= 8)
            {
                unsigned __int128 prod = (unsigned __int128)cache[i-1] * (unsigned __int128)num;
                val = (seq_t)(prod / den);
            }
            else
            {
                val = (cache[i-1] * num) / den;
            }
        }
        else                                 /* i = 2n+1 */
        {
            idx_t k  = (i - 1) >> 1;      /* n in the recurrence */
            seq_t num = (seq_t)(4 * k + 2);
            seq_t den = (seq_t)(k + 1);
            if (sizeof(seq_t) <= 8)
            {
                unsigned __int128 prod = (unsigned __int128)cache[i-1] * (unsigned __int128)num;
                val = (seq_t)(prod / den);
            }
            else
            {
                val = (cache[i-1] * num) / den;
            }
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
