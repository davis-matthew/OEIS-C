/*
 * A021332  Decimal expansion of 1/328.
 * https://oeis.org/A021332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021332
    Decimal expansion of 1/328.
    a(n) = digit n after the decimal point of 1/328 (0‑based)
          = ⌊10^(n+1)/328⌋ mod 10
    This implementation computes digits sequentially
    and memoises them for arbitrary order of calls.  */
#include <stdint.h>
#include <stdlib.h>

/* external linkage */
seq_t a(idx_t n)
{
    /* cache of computed digits */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of digits cached so far */
    static idx_t cache_cap = 0;      /* capacity of the cache */
    /* remainder after the last computed digit (mod 328) */
    static uint64_t rem = 1;          /* rem < 328 */

    /* grow the cache if needed */
    if (cache_len == cache_cap)
    {
        idx_t new_cap = cache_cap == 0 ? 64 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 as safe default */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute digits up to n */
    while (cache_len <= n)
    {
        rem *= 10ULL;
        seq_t digit = (seq_t)(rem / 328ULL);   /* digit (0…9) */
        rem = rem % 328ULL;
        cache[cache_len++] = digit;
    }

    return cache[n];
}
