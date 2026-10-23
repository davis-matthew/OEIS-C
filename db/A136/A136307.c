/*
 * A136307  a(n)=n*(10^K) + a(n-1); a(0)=1; K=floor(log_10 a(n-1))+1.
 * https://oeis.org/A136307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 10^(digit_length(a(n-1))) + a(n-1), a(0) = 1 *
 *            (digit_length counts decimal digits of a(n-1)). * Example terms:
 *            * a(0)=1, a(1)=11, a(2)=211,
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"   /* pow10(idx_t e) → seq_t */

/* Count decimal digits of x.  Assumes x > 0. */
static int digit_count(seq_t x)
{
    int d = 0;
    while (x > 0) {
        x /= (seq_t)10;
        ++d;
    }
    return d;
}

/* Memoised computation of a(n).  The function is thread‑unsafe but correct
 * for any sequence of calls in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoisation array               */
    static size_t cap    = 0;       /* current allocation size (bytes) */
    static size_t len    = 0;       /* number of values already cached */

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failure: abort */
            exit(EXIT_FAILURE);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise a(0) if not yet done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Return cached value if available */
    if (n < len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) continue;          /* already initialised */
        seq_t prev = cache[i - 1];
        int k = digit_count(prev);
        seq_t pow10k = pow10((idx_t)k); /* 10^k */
        seq_t term = ((seq_t)i) * pow10k;
        cache[i] = prev + term;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
