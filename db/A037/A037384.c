/*
 * A037384  Numbers k such that every base-3 digit of k is a base-8 digit of
 *          k.
 * https://oeis.org/A037384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that every digit of k in
 *            base‑3 occurs as a digit of k in base‑8
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence by brute‑force search: for each
   integer k, extract its base‑3 digits and its base‑8 digits; if every
   base‑3 digit appears somewhere in the base‑8 representation,
   k is accepted.  The routine keeps a cached list of found terms,
   extending it as needed up to the requested index n.  This approach
   is O(n log k) and runs well below the time limit for n up to about 10000. */

#include <stdbool.h>
#include <stdlib.h>

/* The function signature required by the OEIS harness. */
seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *cache = NULL;     /* pointer to cached values */
    static idx_t cache_len = 0;     /* number of elements stored */
    static idx_t cache_cap = 0;     /* current capacity of the cache */
    static seq_t cur = 0;           /* last value examined (not yet in cache) */

    if (n == 0)                /* offset 1: a(1) is first term */
        return 0;

    /* Extend cache until it contains the requested index */
    while (cache_len < n)
    {
        /* find next integer satisfying the digit property */
        for (;;)
        {
            cur += 1;

            /* collect base‑3 digits */
            bool has3[3] = {false, false, false};
            seq_t t = cur;
            while (t > 0)
            {
                int d = (int)(t % 3);
                has3[d] = true;
                t /= 3;
            }

            /* collect base‑8 digits */
            bool has8[8] = {false, false, false, false,
                            false, false, false, false};
            t = cur;
            while (t > 0)
            {
                int d = (int)(t % 8);
                has8[d] = true;
                t /= 8;
            }

            /* check inclusion condition */
            bool ok = true;
            for (int d = 0; d < 3; ++d)
            {
                if (has3[d] && !has8[d])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;
        }

        /* Grow cache if necessary */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp) /* out of memory */
            {
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store accepted number */
        cache[cache_len++] = cur;
    }

    /* Return the required term (1‑based index) */
    return cache[n - 1];
}
