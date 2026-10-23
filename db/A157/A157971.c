/*
 * A157971  Odious twin locations: first members of pairs of consecutive
 *          odious numbers.
 * https://oeis.org/A157971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A248057(n) - 1
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the first member of a pair of consecutive odious numbers.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Count 1‑bits in any unsigned seq_t type (built‑in or big integer).  */
static inline unsigned popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x)
    {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* Return true if x has an odd number of 1‑bits (odious).  */
static inline bool is_odious(seq_t x)
{
    return (popcount_seq_t(x) & 1U) != 0;
}

/* Computes the n‑th odious twin location (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)                   /* undefined, but defined to avoid UB */
        return 0;

    /* Cache of already computed terms.  */
    static seq_t *cache   = NULL;
    static size_t  clen   = 0;   /* number of cached terms   */
    static size_t  cap    = 0;   /* capacity of the array   */

    /* State used to generate subsequent terms.  */
    static bool   prev_odious = false;
    static seq_t current     = 1;   /* next candidate integer   */

    /* Ensure cache can hold the requested index.  */
    if (clen < (size_t)n)
    {
        size_t needed = (size_t)n;
        if (cap < needed)
        {
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap < needed)
                newcap <<= 1;

            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                abort();           /* allocation failure  */
            cache = tmp;
            cap   = newcap;
        }

        /* Generate terms until we have enough.  */
        while (clen < (size_t)n)
        {
            if (is_odious(current) && prev_odious)
                cache[clen++] = current - 1;   /* first member of twin */

            prev_odious = is_odious(current);
            current++;
        }
    }

    return cache[n - 1];
}
