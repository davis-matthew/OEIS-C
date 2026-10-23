/*
 * A031590  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 92.
 * https://oeis.org/A031590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A031590: Numbers k such that the continued fraction for sqrt(k)
   has even period and if the last term of the periodic part is
   deleted the central term is 92.
   The implementation enumerates integers k, tests the stated
   property, and caches the results so that repeated calls are fast.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_sqrt.h"

/* Grow a dynamic array of uint64_t */
static void grow_uint64(uint64_t **arr, size_t *cap, size_t *len, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = *cap ? *cap : 8;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    *arr = tmp;
    *cap = new_cap;
}

/* Test whether a given k satisfies the property */
static bool holds_property(uint64_t k)
{
    uint64_t a0 = int_sqrt(k);
    if (a0 * a0 == k) return false;          /* perfect square */

    static uint64_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    terms_len = 0;
    uint64_t m = 0, d = 1, a = a0;
    while (true)
    {
        /* m = d*a - m */
        unsigned __int128 tmp = (unsigned __int128)d * a - m;
        m = (uint64_t)tmp;

        /* d = (k - m*m) / d */
        unsigned __int128 diff = (unsigned __int128)k - (unsigned __int128)m * m;
        d = (uint64_t)(diff / d);

        /* a = (a0 + m) / d */
        a = (a0 + m) / d;

        grow_uint64(&terms, &terms_cap, &terms_len, terms_len + 1);
        terms[terms_len++] = a;

        if (a == 2 * a0) break;    /* period finished */
    }

    /* Even period?  If so, check the central term after deleting last */
    if ((terms_len & 1) == 0)
    {
        size_t target = terms_len / 2 - 1;
        return terms[target] == 92;
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined position */

    /* Static cache of found k values */
    static uint64_t *cache = NULL;
    static size_t cap = 0;
    static size_t size = 0;
    static uint64_t last_checked = 2;   /* next k to test */

    while (size < (size_t)n)
    {
        uint64_t k = last_checked++;
        if (holds_property(k))
        {
            grow_uint64(&cache, &cap, &size, size + 1);
            cache[size++] = k;
        }
    }
    return (seq_t)cache[n - 1];
}
