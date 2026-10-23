/*
 * A044683  Numbers n such that string 5,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers whose base‑9 representation contains
   the substring “56” but the following integer does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return true if x written in base 9 contains the pattern 5,6
   with 5 being the more significant digit. */
static bool has56(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    const seq_t nine = (seq_t)9;
    int prev = -1;                 /* previous (less‑significant) digit */

    while (x > (seq_t)0)
    {
        int digit = (int)(x % nine);
        x /= nine;
        /* pattern 5 (more significant) followed by 6 (less significant)
           is found when the previous digit equals 6 and the current
           digit equals 5 as we walk from least‑significant to most‑significant. */
        if (prev == 6 && digit == 5)
            return true;
        prev = digit;
    }
    return false;
}

#ifdef __cplusplus
/* C++ version: use std::vector for caching */
static std::vector<seq_t> cache;
static seq_t next_test = (seq_t)1;
#else
/* C version: dynamic array of seq_t */
static seq_t *cache      = NULL;
static size_t cap        = 0;
static size_t count      = 0;       /* number of cached terms */
static seq_t next_test    = (seq_t)1;
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n <= 0 in OEIS */

#ifdef __cplusplus
    while (cache.size() < n)
    {
        seq_t cur = next_test;
        seq_t nxt = next_test + (seq_t)1;
        if (has56(cur) && !has56(nxt))
            cache.push_back(cur);
        ++next_test;
    }
    return cache[n - 1];
#else
    while (count < n)
    {
        seq_t cur = next_test;
        seq_t nxt = next_test + (seq_t)1;
        if (has56(cur) && !has56(nxt))
        {
            if (count == cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();            /* allocation failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[count++] = cur;
        }
        ++next_test;
    }
    return cache[n - 1];
#endif
}
