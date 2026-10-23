/*
 * A033900  Sort then Add, a(1)=19.
 * https://oeis.org/A033900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=19; a(n)=a(n-1)+sort_ascending_digits(a(n-1)) for n>1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the integer formed by sorting the decimal digits of x
   in ascending order (leading zeros omitted). */
static seq_t ascending_digits(seq_t x)
{
    if (x == 0)
        return 0;

    unsigned int freq[10] = {0};
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        freq[d]++;
        x /= 10;
    }

    seq_t y = 0;
    for (unsigned int d = 0; d <= 9; ++d) {
        unsigned int c = freq[d];
        while (c--) {
            y = y * 10 + d;
        }
    }
    return y;
}

/* Compute the nth term of A033900.  Results are cached so that
   subsequent calls are fast regardless of call order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* First call: initialise storage for a(1). */
    if (!cache) {
        cache_cap = 2;            /* indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;            /* allocation failure */
        cache[0] = 0;            /* unused */
        cache[1] = (seq_t)19;    /* a(1) */
        cache_len = 1;
    }

    /* Grow cache if necessary and compute missing terms. */
    if (n > cache_len) {
        size_t needed = (size_t)n + 1;          /* indices 0..n */
        if (needed > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp)
                return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = needed;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + ascending_digits(cache[i - 1]);
        }
        cache_len = n;
    }

    return cache[n];
}
