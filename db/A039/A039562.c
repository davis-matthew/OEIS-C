/*
 * A039562  Numbers whose base-4 representation has the same number of 1's,
 *          2's and 3's.
 * https://oeis.org/A039562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑4 representation contains equal numbers of
   digits 1, 2 and 3.  The implementation generates terms by testing
   successive integers and caching those that satisfy the condition. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache storage for the sequence (provided types seq_t and idx_t are
   defined by the build environment). */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* current allocated capacity */
static seq_t next_candidate = 1;     /* next integer to test (after 0) */

/* Ensure that the cache has room for at least `need` elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a number has the same number of 1’s, 2’s and 3’s in base‑4. */
static bool good(seq_t x)
{
    unsigned cnt[4] = {0, 0, 0, 0};
    while (x > 0)
    {
        unsigned d = (unsigned)(x % (seq_t)4);
        ++cnt[d];
        x /= (seq_t)4;
    }
    return (cnt[1] == cnt[2]) && (cnt[1] == cnt[3]);
}

/* Exported sequence function: returns the n‑th term (a(1) = 0). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined below, return a placeholder */

    /* Initialize the cache upon first use. */
    if (cache_len == 0)
    {
        ensure_capacity(1);
        cache[0] = 0;
        cache_len = 1;
        next_candidate = 1;  /* start testing from 1 */
    }

    /* Generate terms until we have at least n terms cached. */
    while ((size_t)n > cache_len)
    {
        if (good(next_candidate))
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
