/*
 * A226370  Number of tilings of a 6 X n rectangle using integer-sided square
 *          tiles of area > 1.
 * https://oeis.org/A226370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+2*a(n-4)+a(n-6) for n>5, with a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=3, a(5)=2
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 49, u64 n <= 96, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void
ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    /* need room for indices 0 .. n inclusive */
    size_t needed = (size_t)n + 1;
    if (*cap_ptr < needed) {
        size_t new_cap = (*cap_ptr == 0) ? 8 : *cap_ptr;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        *cache_ptr = tmp;
        *cap_ptr   = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed_upto = 0;   /* largest index already computed */

    /* make sure memory is big enough */
    ensure_cache(n, &cache, &cap);

    /* initialise base cases up to n (in case we just grew the array) */
    if (n >= 0) cache[0] = 1;
    if (n >= 1) cache[1] = 0;
    if (n >= 2) cache[2] = 1;
    if (n >= 3) cache[3] = 1;
    if (n >= 4) cache[4] = 3;
    if (n >= 5) cache[5] = 2;

    /* compute missing values using the recurrence */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i >= 6) {
            cache[i] = cache[i-2] + cache[i-3] +
                       (seq_t)2 * cache[i-4] + cache[i-6];
        }
    }

    if (n > computed_upto) computed_upto = n;
    return cache[n];
}
