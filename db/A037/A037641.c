/*
 * A037641  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1.
 * https://oeis.org/A037641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A037641: a(1)=2; for n>1, a(n)=a(n-1)*6 + d,
   where d is the (n-1)th digit of the repeating pattern 2,3,1 (period 3). */
#include <stdlib.h>

static const seq_t period_digits[3] = { 2, 3, 1 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 – return 0 for safety */

    /* static cache for memoised values, indices start at 0 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* size of cache array (indices 0..cap-1) */
    static size_t cache_len = 0;   /* highest index that has been computed */

    /* enlarge cache to at least index n */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise the first term if the cache is empty */
    if (cache_len == 0) {
        cache[0] = 0;      /* unused, keeps the array non‑empty */
        cache[1] = 2;      /* a(1) */
        cache_len = 1;
    }

    /* compute all missing terms up to n */
    while (cache_len < n) {
        size_t idx = cache_len + 1;                      /* next index */
        seq_t prev = cache[cache_len];                    /* a(idx-1) */
        seq_t d   = period_digits[(idx - 1) % 3];         /* digit pattern */
        cache[idx] = prev * 6 + d;                        /* a(idx)  */
        cache_len = idx;
    }

    return cache[n];
}
