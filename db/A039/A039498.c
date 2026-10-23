/*
 * A039498  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 6's.
 * https://oeis.org/A039498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th number whose base‑12 representation has the same
 *            * nonzero number of 0's and 6's
 * Range:     u8 n <= 3, u16 n <= 3718, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Test whether v has equal, non‑zero counts of 0 and 6 in base 12. */
static bool equal_zero_and_six(seq_t v)
{
    unsigned count0 = 0, count6 = 0;
    while (v > (seq_t)0)
    {
        unsigned digit = (unsigned)(v % (seq_t)12);
        if (digit == 0)
            ++count0;
        else if (digit == 6)
            ++count6;
        v /= (seq_t)12;
    }
    return (count0 == count6) && (count0 > 0);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* index 0 is outside the sequence */

#if defined(__cplusplus)

    static seq_t last_val = 0;
    static std::vector<seq_t> cache;

    if (cache.size() < n)
    {
        while (cache.size() < n)
        {
            ++last_val;
            if (equal_zero_and_six(last_val))
                cache.push_back(last_val);
        }
    }
    return cache[n - 1];

#else   /* C implementation */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0, cache_cap = 0;
    static seq_t last_val = 0;

    if (cache_len < n)
    {
        if (cache == NULL)
        {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache)
                exit(EXIT_FAILURE);
        }
        while (cache_len < n)
        {
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
            }
            ++last_val;
            if (equal_zero_and_six(last_val))
                cache[cache_len++] = last_val;
        }
    }

    return cache[n - 1];

#endif
}
