/*
 * A155986  a(1), then a(n) = smallest cube not occurring earlier, not ending
 *          with zero and starting with the last digit of a(n-1).
 * https://oeis.org/A155986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   used_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A155986
   a(1) = 1,
   a(n) = smallest cube not occurring earlier,
   that does not end with 0 and starts with the last decimal digit
   of a(n‑1). */

/* The implementation keeps a cache of computed terms.  For each new
   term it scans cube roots from 1 upwards until it finds the first
   cube that satisfies all the conditions and has not appeared before. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/used_contains.h"

/* ----------  Helpers  ---------- */

static int first_digit(seq_t v)
{
    while (v >= (seq_t)10)
        v /= (seq_t)10;
    return (int)v;
}

/* ----------  Global cache (static)  ---------- */

static seq_t *cache        = NULL;   /* a(1), a(2), … */
static size_t cache_len    = 0;      /* number of terms computed */
static size_t cache_cap    = 0;      /* allocated capacity */

static seq_t *used         = NULL;   /* cubes that have already appeared */
static size_t used_len     = 0;
static size_t used_cap     = 0;

/* Ensure the cache has space for at least new_len elements. */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < new_len)
        new_cap = new_len;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the used table has space. */
static void grow_used(size_t new_len)
{
    if (new_len <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < new_len)
        new_cap = new_len;
    seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    used = tmp;
    used_cap = new_cap;
}

/* ----------  Main function  ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n<1, return 0 */

    /* allocate cache space */
    grow_cache((size_t)n);

    /* compute until we have n terms */
    while (cache_len < n)
    {
        /* Special case: first term */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)1;
            grow_used(1);
            used[0] = (seq_t)1;
            ++cache_len;
            ++used_len;
            continue;
        }

        /* last term's last digit */
        seq_t last = cache[cache_len - 1];
        int last_digit = (int)(last % (seq_t)10);

        /* search for next cube */
        seq_t next_cube = (seq_t)0;
        for (idx_t root = 1; ; ++root)
        {
            seq_t r = (seq_t)root;
            seq_t cube = r * r * r;          /* r^3 */

            /* reject cubes ending in 0 */
            if ((cube % (seq_t)10) == (seq_t)0)
                continue;

            /* reject if first digit does not match last_digit */
            if (first_digit(cube) != last_digit)
                continue;

            /* reject if already used */
            if (used_contains(used, used_len, cube))
                continue;

            /* accept */
            next_cube = cube;
            break;
        }

        /* store the new term */
        cache[cache_len] = next_cube;
        grow_used(used_len + 1);
        used[used_len] = next_cube;
        ++cache_len;
        ++used_len;
    }

    return cache[n - 1];
}
