/*
 * A118160  Start with 1 and repeatedly reverse the digits and add 48 to get
 *          the next term.
 * https://oeis.org/A118160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = rev(a(n-1)) + 48   (decimal digits reversed) */
#include <stddef.h>
#include <stdlib.h>

/* Reverse the decimal digits of x. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x != 0)
    {
        seq_t d = x % 10;
        rev = rev * 10 + d;
        x /= 10;
    }
    return rev;
}

/* Cache for memoisation. */
static seq_t *cache      = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)  /* out of memory: leave cache unchanged */
        return;
    if (cache)
    {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, return 0 */

    size_t idx = (size_t)n;   /* idx >= 1 */
    ensure_cache(idx);
    if (cache_len == 0)
    {
        cache[0] = 1;
        cache_len = 1;
    }
    while (cache_len < idx)
    {
        seq_t prev = cache[cache_len - 1];
        seq_t next = reverse_digits(prev) + (seq_t)48;
        cache[cache_len] = next;
        ++cache_len;
    }
    return cache[idx - 1];
}
