/*
 * A039495  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 3's.
 * https://oeis.org/A039495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039495: Numbers whose base‑12 representation has the same
 * nonzero number of 0s and 3s.
 * Computation: incremental search for qualifying numbers. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;

static inline bool qualifies(seq_t x)
{
    unsigned count0 = 0, count3 = 0;
    seq_t t = x;
    if (t == 0)
        return false;
    while (t > 0) {
        unsigned digit = (unsigned)(t % 12);
        if (digit == 0)
            ++count0;
        else if (digit == 3)
            ++count3;
        t /= 12;
    }
    return (count0 > 0 && count0 == count3);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (cache_len >= n)
        return cache[n - 1];
    while (cache_len < n) {
        while (!qualifies(next_candidate))
            ++next_candidate;
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
