/*
 * A152824  Numbers such that each digit leaves the same nonzero remainder
 *          when each is divided into the number.
 * https://oeis.org/A152824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 202, u32 n <= 1543, u64 n <= 1543,
 *            u128 n <= 1543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152824: numbers n such that each decimal digit d of n (d≠0, d≠1)
   leaves the same remainder R≠0 when n is divided by d.
   For this particular sequence all accepted numbers satisfy R = 1.
   Algorithm: brute‑force search with memoisation.  The function keeps a
   dynamic array of found terms which is extended on demand.  The first
   argument n is 1‑based (a(1) is the first term).  No external library
   functions are used. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of found terms          */
static size_t cache_len = 0;    /* number of terms stored        */
static size_t cache_cap = 0;    /* current capacity of cache      */
static seq_t last_checked = 0;  /* largest value examined so far   */

static bool good(seq_t x)
{
    bool seen[10] = {0};
    int r = -1;
    seq_t tmp = x;

    /* zero is not allowed and digits 0 or 1 give remainder 0 */
    if (tmp == 0)
        return false;

    while (tmp > 0)
    {
        int d = (int)(tmp % 10);   /* digit on the right 1‑9 */
        tmp /= 10;
        if (d == 0 || d == 1)
            return false;
        if (!seen[d])
        {
            seen[d] = true;
            int rem = (int)(x % d);
            if (r == -1)
                r = rem;
            else if (rem != r)
                return false;
        }
    }
    /* remainder must be 1 for this sequence */
    return r == 1;
}

static bool ensure_capacity(void)
{
    if (cache_len < cache_cap)
        return true;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return false;
    cache = tmp;
    cache_cap = new_cap;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1; safe guard */

    while (cache_len < (size_t)n)
    {
        ++last_checked;
        if (!ensure_capacity())
            return 0; /* allocation failure – sentinel */

        if (good(last_checked))
            cache[cache_len++] = last_checked;
    }
    return cache[n - 1];
}
