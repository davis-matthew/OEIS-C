/*
 * A037220  Summarize the previous term!.
 * https://oeis.org/A037220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0; for n>0, a(n) is obtained by counting the occurrences
 *            of each digit in a(n-1) and concatenating the counts and the
 *            digit numbers in ascending order of the digit value
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"

static seq_t *cache = NULL;    /* array of cached terms */
static size_t cache_len = 0;   /* number of terms currently cached (indices 0 .. cache_len-1) */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* helper: number of decimal digits of a non‑negative integer */
static inline int int_digits(uint64_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* ensure the cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                           /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* make sure we have storage for all terms up to n */
    ensure_capacity(n);

    /* initialise base case if needed */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        int cnt[10] = {0};

        if (prev == (seq_t)0)
        {
            cnt[0] = 1;                       /* the single digit '0' */
        }
        else
        {
            seq_t t = prev;
            while (t > (seq_t)0)
            {
                int digit = (int)(t % (seq_t)10);
                ++cnt[digit];
                t /= (seq_t)10;
            }
        }

        seq_t val = 0;
        for (int d = 0; d < 10; ++d)
        {
            int c = cnt[d];
            if (c == 0) continue;

            int len_c = int_digits((uint64_t)c);
            seq_t mult = pow10((idx_t)(len_c + 1));
            val = val * mult + (seq_t)c * (seq_t)10 + (seq_t)d;
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
