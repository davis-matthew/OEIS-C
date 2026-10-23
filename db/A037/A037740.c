/*
 * A037740  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A037740 – OEIS sequence “Base-7 digits are, in order, the first n terms of
    the periodic sequence 2,1,3,0.”  

    Recurrence:
        a(1) = 2
        a(n) = a(n-1) * 7 + d,
        d = {2,1,3,0}[(n-1) mod 4]
    The function memoises previously computed values so that calls can
    arrive in any order. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values, 0‑based index = a(idx+1) */
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* current allocation capacity */

/* Ensure that the cache can hold at least `need` entries. */
static void
ensure_cache_size(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A037740 (a(1)=2, ...).  n is 1‑based. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;        /* out of bounds – should not happen */
    if (cache_len == 0)         /* initialise with first term */
    {
        ensure_cache_size(1);
        cache[0] = 2;
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n > cache_len)
    {
        ensure_cache_size((size_t)n);
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i)
        {
            idx_t pos = (i - 1) % 4;
            seq_t digit;
            switch (pos) {
                case 0: digit = 2; break;
                case 1: digit = 1; break;
                case 2: digit = 3; break;
                default: digit = 0; /* pos == 3 */
            }
            seq_t prev = cache[i - 2];        /* a(i-1) */
            cache[i - 1] = prev * (seq_t)7 + digit;
        }
        cache_len = (size_t)n;
    }
    return cache[n - 1];
}
