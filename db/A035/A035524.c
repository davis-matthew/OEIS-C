/*
 * A035524  Reverse and add (in base 4).
 * https://oeis.org/A035524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(0) = 1; a(n+1) = reverse_and_add_4(a(n)) (reverse+add in base
 *            4)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 31, u64 n <= 62, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* reverse the digits of x in base 4 */
static seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    while (x != 0)
    {
        seq_t digit = x % 4;
        rev = rev * 4 + digit;
        x /= 4;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialize cache for a(0) */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache_len = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */
        cache[0] = 1;
    }

    if ((size_t)n >= cache_len)
    {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap)
        {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();        /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t rev  = reverse_base4(prev);
            cache[i] = prev + rev;
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
