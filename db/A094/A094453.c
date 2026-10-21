/*
 * A094453  Numbers k such that binomial(2*k, k)/(k+2) is not an integer.
 * https://oeis.org/A094453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A094453
    Numbers k such that binomial(2*k, k)/(k+2) is not an integer.
    The condition is equivalent to:
        - (k+2) is a power of 2
        - OR 3 divides (k+2) and, in base‑3, every digit is 0 or 1
          except possibly the 3¹ place, which may be 2.
    The implementation builds the sequence lazily, caching already
    computed terms.  Cached values are stored in a dynamic array that
    grows only when necessary.  The algorithm runs in O(n) time for
    the first n terms. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_power_of_two.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* External linkage is required. */
seq_t a(idx_t n)
{
    /* Static cache of discovered terms. */
    static seq_t *cache      = NULL;   /* array holding terms */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static size_t cache_len  = 0;      /* number of terms stored so far */
    static idx_t  next_k      = 1;     /* next k to test */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cache_cap = 256;                /* start with some space */
        cache = alloc_seq_array(cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* m = k + 2, the number we test. */
        seq_t m = (seq_t)next_k + (seq_t)2;
        bool good = false;

        /* First condition: (k+2) is a power of 2. */
        if (is_power_of_two(m))
            good = true;
        else if ((m % (seq_t)3) == (seq_t)0) {   /* m divisible by 3 */
            seq_t tmp = m;
            bool bad = false;
            size_t pos = 0;                     /* digit position in base 3 */
            while (tmp > (seq_t)0) {
                seq_t d = tmp % (seq_t)3;       /* current base-3 digit */
                if (pos != 1 && d == (seq_t)2) {
                    bad = true;                 /* illegal digit */
                    break;
                }
                tmp /= (seq_t)3;
                pos++;
            }
            if (!bad)
                good = true;
        }

        /* If the condition holds we keep this k. */
        if (good) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *newp = alloc_seq_array(new_cap);
                /* copy old contents */
                for (size_t i = 0; i < cache_len; ++i)
                    newp[i] = cache[i];
                free_seq_array(cache);
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }

        next_k++;   /* advance to next candidate k */
    }

    /* Return the requested term. */
    return cache[n - 1];
}
