/*
 * A043500  Numbers having four 2's in base 10.
 * https://oeis.org/A043500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose decimal representation
 *            contains exactly four digit '2's (OEIS sequence A043500)
 * Range:     u8 none, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Counts the number of digit '2' in the decimal representation of x. */
static inline int count_twos(idx_t x)
{
    int c = 0;
    while (x != 0)
    {
        if ((x % 10) == 2)
            c++;
        x /= 10;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* Undefined for offset 1; safe guard */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* capacity of cache array   */
    static size_t sz  = 0;          /* number of terms stored    */
    static idx_t last_val = 0;      /* numeric value of last term */

    if (n > sz)
    {
        /* Ensure enough space in the cache */
        size_t need = n;
        if (need > cap)
        {
            size_t nc = cap == 0 ? 16 : cap * 2;
            while (nc < need)
                nc *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, nc * sizeof(seq_t));
            if (!tmp)
            {
                /* Allocation failure; terminate gracefully */
                return (seq_t)0;
            }
            cache = tmp;
            cap   = nc;
        }

        /* Generate terms until we reach the requested index */
        idx_t candidate = last_val;
        while (sz < need)
        {
            /* Find the next integer having exactly four '2's */
            do
                ++candidate;
            while (count_twos(candidate) != 4);

            cache[sz++] = (seq_t)candidate;  /* store the term   */
            last_val   = candidate;          /* update the last value */
        }
    }

    return cache[n - 1];   /* 1-based offset: a(1) is cache[0] */
}
