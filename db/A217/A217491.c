/*
 * A217491  Next largest positive integer without a digit 1 and not divisible
 *          by any digit of the preceding term.
 * https://oeis.org/A217491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Next largest positive integer without a digit 1 and not divisible by any digit of the preceding term. */
/* The function builds the sequence on demand, memoising previous results. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t built = 0; /* number of terms stored (>=0) */

    /* Initialise cache if not yet done */
    if (!cache)
    {
        capacity = 64;
        cache   = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed – unlikely in tests */
    }

    /* Ensure the first term a(1)=2 is stored */
    if (built == 0)
    {
        cache[0] = 2;
        built   = 1;
    }

    /* Extend cache up to requested index n */
    while (built < n)
    {
        seq_t prev      = cache[built - 1];
        seq_t candidate = prev + (seq_t)1;

        while (1)
        {
            /* Reject numbers containing the digit 1 */
            seq_t tmp = candidate;
            bool has1 = false;
            while (tmp > (seq_t)0)
            {
                unsigned d = (unsigned)(tmp % (seq_t)10);
                if (d == 1)
                {
                    has1 = true;
                    break;
                }
                tmp /= (seq_t)10;
            }
            if (has1)
            {
                ++candidate;
                continue;
            }

            /* Reject numbers divisible by any non‑zero digit of 'prev' */
            seq_t tprev = prev;
            bool bad    = false;
            while (tprev > (seq_t)0)
            {
                unsigned d = (unsigned)(tprev % (seq_t)10);
                if (d > 0 && (candidate % (seq_t)d) == (seq_t)0)
                {
                    bad = true;
                    break;
                }
                tprev /= (seq_t)10;
            }
            if (bad)
            {
                ++candidate;
                continue;
            }

            /* candidate is valid */
            break;
        }

        /* Grow cache if necessary */
        if ((size_t)built >= capacity)
        {
            capacity *= 2;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!cache)
                return 0; /* allocation failed */
        }

        cache[built] = candidate;
        ++built;
    }

    return cache[n - 1];
}
