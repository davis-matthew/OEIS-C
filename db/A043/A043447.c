/*
 * A043447  Numbers having three 6's in base 8.
 * https://oeis.org/A043447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_six_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer whose base‑8 representation contains exactly three 6's. */

#include <stdlib.h>
#include "../utils/count_six_digits.h"

/* Memoisation state. */
static seq_t *cache      = NULL;   /* stored terms */
static size_t cache_cap  = 0;      /* allocated length of cache */
static size_t cache_len  = 0;      /* number of terms stored */
static seq_t  next_val   = 0;      /* next integer to examine */

/* Ensure that the cache holds at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure */

    cache      = tmp;
    cache_cap  = new_cap;
}

/* Public entry point as required. */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                    /* behaviour for n<1 is irrelevant */

    size_t need = (size_t)n;
    if (need <= cache_len)          /* already computed */
        return cache[need - 1];

    /* Generate successive terms until we have at least 'n' of them. */
    while (cache_len < need)
    {
        seq_t val = next_val++;
        if (count_six_digits(val) == 3)
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
    }
    return cache[need - 1];
}
