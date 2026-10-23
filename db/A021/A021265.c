/*
 * A021265  Decimal expansion of 1/261.
 * https://oeis.org/A021265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit of the decimal expansion of 1/261 (0‑based
 *            indexing). Equivalent: a(n) = floor(10^(n+1)/261) mod 10
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
 * Cached digits for repeated queries.
 * `cache` stores already computed digits.
 * `cache_len` is the number of digits stored (always equals the
 * number of valid entries in `cache`).
 * `cache_cap` is the allocated capacity of `cache`.
 * `rem` holds the current remainder after the last computed digit
 * (i.e. after digit a(cache_len-1)). Initially rem = 1, representing
 * 1/261.
 */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t rem = 1;   /* remainder after the previously computed digit */

/* Ensure the cache has capacity for at least `need` entries */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n-th decimal digit of 1/261 (n ≥ 0). */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cache_capacity(n + 1);

    /* Compute new digits from cache_len to n inclusive. */
    for (idx_t i = cache_len; i <= n; ++i) {
        rem = rem * (seq_t)10;                /* advance to next digit position */
        seq_t digit = rem / (seq_t)261;       /* digit at position i */
        rem %= (seq_t)261;                    /* update remainder for next step */
        cache[i] = digit;
    }

    cache_len = n + 1;
    return cache[n];
}
