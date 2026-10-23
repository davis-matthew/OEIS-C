/*
 * A043584  Numbers whose base-3 representation has exactly 4 runs.
 * https://oeis.org/A043584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑3 representation
 *            consists of exactly four runs of consecutive equal digits. The
 *            function generates terms incrementally and caches them
 * Range:     u8 n <= 86, u16 n <= 3465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count the runs in the base‑3 expansion of x. */
static inline int count_runs(seq_t x)
{
    int runs = 0;
    if (x == 0)
        return 0;
    while (x)
    {
        seq_t digit = x % 3;
        ++runs;
        do
        {
            x /= 3;
        } while (x && (x % 3) == digit);
    }
    return runs;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_len = 0;    /* number of terms stored */
    static seq_t cur = 0;           /* last integer examined */

    while (cache_len < n)
    {
        ++cur;                       /* examine next integer */
        if (count_runs(cur) == 4)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)           /* allocation failed – abort */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur; /* store the term */
        }
    }
    return cache[n - 1];
}
