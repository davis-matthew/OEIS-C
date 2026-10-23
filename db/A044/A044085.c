/*
 * A044085  Numbers k such that string 1,3 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that the base‑5 representation of k contains the
 *            * substring 13, while the base‑5 representation of k-1 does NOT
 *            contain * that substring. Computed by brute‑force search with
 *            memoisation
 * Range:     u8 n <= 12, u16 n <= 2781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true exactly when base‑5 representation of x contains the
 * two‐digit substring 13. */
static inline bool contains13(seq_t x)
{
    unsigned last = 5;            /* sentinel that cannot be a real digit */
    bool found = false;

    while (x != (seq_t)0)
    {
        unsigned digit = (unsigned)(x % (seq_t)5);
        x = x / (seq_t)5;
        if (last == 3 && digit == 1)
            found = true;
        last = digit;
    }
    return found;
}

seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for index 0 – safe return */
        return (seq_t)0;

    static seq_t *cache = NULL;      /* array storing found terms     */
    static size_t cache_cap = 0;     /* current capacity              */
    static idx_t cached_n = 0;       /* number of terms cached        */
    static seq_t curr_k = (seq_t)1;  /* next integer to test          */

    /* Grow cache array if necessary.  The cast is required for C++17. */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                    /* allocation failure – fatal */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we've reached the requested index. */
    while (cached_n < n)
    {
        if (contains13(curr_k) &&
            !contains13(curr_k - (seq_t)1))
        {
            cache[cached_n] = curr_k;
            ++cached_n;
        }
        ++curr_k;
    }

    return cache[(size_t)(n - 1)];
}
