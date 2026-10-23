/*
 * A037380  Numbers whose base-3 digits are all present among their base-4
 *          digits.
 * https://oeis.org/A037380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑3 digits are all present among their base‑4 digits.
   The function examines successive integers, storing each qualifying number
   in a static cache so that subsequent calls are answered in O(1) on average.
   The cache is grown with realloc, which is safe for both the integer
   and arbitrary‑precision builds when an explicit cast is used. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Static cache of found terms.  */
static seq_t *cache = NULL;
static size_t cache_size = 0;  /* number of terms stored */
static size_t cache_cap   = 0;  /* current capacity of cache */

/* Ensure the cache has at least 'need' slots.  */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();          /* unrecoverable allocation failure */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Return true iff every digit that appears in the base‑3 representation
   of 'num' also appears in its base‑4 representation. */
static bool matches(seq_t num)
{
    bool has3[3] = {false, false, false};
    bool has4[4] = {false, false, false, false};

    seq_t tmp = num;
    if (tmp == 0)
        has3[0] = true;
    else
        while (tmp)
        {
            unsigned d = (unsigned)(tmp % 3);
            has3[d] = true;
            tmp /= 3;
        }

    tmp = num;
    if (tmp == 0)
        has4[0] = true;
    else
        while (tmp)
        {
            unsigned d = (unsigned)(tmp % 4);
            has4[d] = true;
            tmp /= 4;
        }

    for (unsigned d = 0; d < 3; ++d)
        if (has3[d] && !has4[d])
            return false;
    return true;
}

/* Main sequence function requested.  */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – undefined for index 0 */
        return 0;

    static seq_t next_candidate = 1;  /* integer to test next */
    while (cache_size < (size_t)n)
    {
        /* Find next number satisfying the property. */
        while (true)
        {
            if (matches(next_candidate))
            {
                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
