/*
 * A144688  "Magic" numbers: all numbers from 0 to 9 are magic; a number >= 10
 *          is magic if it is divisible by the number of its digits and the
 *          number obtained by deleting the final digit is also magic.
 * https://oeis.org/A144688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1046, u32 n <= 9917, u64 n <= 8384,
 *            u128 n <= 8384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Magic numbers:   
 * a(n) = the nth smallest non‑negative integer that is divisible by its
 * number of digits and whose prefix obtained by deleting the final digit
 * is also magic.  The first term is a(1)=0.
 *
 * The implementation builds the sequence incrementally with a breadth‑first
 * search.  Numbers are stored in a static array in ascending order, so
 * any call to a(n) can be answered immediately after the required
 * number of terms have been generated.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Utility: count decimal digits of a seq_t value                */
/* ------------------------------------------------------------ */
static inline unsigned int seq_digit_len(seq_t x)
{
    unsigned int len = 0;
    seq_t tmp = x;
    do
    {
        ++len;
        tmp /= (seq_t)10;
    }
    while (tmp != (seq_t)0);
    return len;
}

/* ------------------------------------------------------------ */
/* Main sequence routine                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but keep safety */
        return 0;

    /* static cache of generated terms */
    static seq_t *cache     = NULL;   /* array of magic numbers   */
    static size_t  cache_len = 0;     /* current number of terms */
    static size_t  cache_cap = 0;     /* allocated capacity       */
    static size_t  pos        = 0;     /* position of next element to process */

    /* initialise on first entry */
    if (!cache)
    {
        cache_cap = 21000;                        /* more than the 20457 terms */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                     /* failure, unlikely */

        /* seed: the single‑digit magic numbers 0 … 9 */
        cache_len = 0;
        for (seq_t i = 0; i <= 9; ++i)
            cache[cache_len++] = i;

        pos = 0;
    }

    /* expandable if necessary */
    if (cache_len >= n)
        return cache[n - 1];

    /* breadth‑first generation until we have at least n numbers   */
    while (cache_len < n)
    {
        /* nothing left to process -> cannot continue */
        if (pos >= cache_len)
            break;

        seq_t cur = cache[pos];

        if (cur != 0)         /* 0 has no meaningful children */
        {
            /* number of digits of cur, and expected length of the child */
            unsigned int cur_len   = seq_digit_len(cur);
            unsigned int child_len = cur_len + 1;

            /* compute cur * 10 safely (no overflow for arbitrary seq_t) */
            seq_t base = cur * (seq_t)10;

            /* children are cur*10 + d for d in 0…9 that are divisible by child_len */
            for (unsigned int d = 0; d < 10; ++d)
            {
                seq_t child = base + (seq_t)d;

                /* Ordinary integer modulo works for all defined seq_t */
                if (child % (seq_t)child_len == (seq_t)0)
                {
                    if (cache_len == cache_cap)            /* grow cache if full */
                    {
                        cache_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!tmp) { free(cache); return 0; }
                        cache = tmp;
                    }
                    cache[cache_len++] = child;           /* append child */
                }
            }
        }

        ++pos;                                         /* move on to next current element */
    }

    /* return the requested term if we managed to generate it */
    if (n <= cache_len)
        return cache[n - 1];

    /* should not happen for requested indices that fit in seq_t */
    return 0;
}
