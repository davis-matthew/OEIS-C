/*
 * A228494  The number of 3-length segments in all possible covers of L-length
 *          line by these segments with allowed gaps < 3.
 * https://oeis.org/A228494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228494: a(n) = 2 a(n-3)+2 a(n-4)+2 a(n-5)-a(n-6)-2 a(n-7)-3 a(n-8)-2 a(n-9)-a(n-10)
   Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=1,a(4)=2,a(5)=3,a(6)=4,a(7)=7
   The recurrence is valid for all n ≥ 8.  The function memoises computed
   values so that calls may arrive in any order.  It uses only integer
   arithmetic on the type seq_t. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Initial known terms 0 … 7. */
static const seq_t init_terms[8] = {0,0,0,1,2,3,4,7};

/* Compute a(n).  Memoises intermediate results. */
seq_t a(idx_t n)
{
    /* Grow cache to accommodate index n. */
    if ((idx_t)(cache_size) <= n)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;  /* allocation failure – caller will handle */
        cache = new_cache;

        /* Build missing terms one by one. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i < 8) /* use explicit initial values */
            {
                cache[i] = init_terms[i];
                continue;
            }

            seq_t val = 0;

            /* Positive contributions: 2*a[i-3] + 2*a[i-4] + 2*a[i-5] */
            if (i >= 3) { val += cache[i - 3]; val += cache[i - 3]; }
            if (i >= 4) { val += cache[i - 4]; val += cache[i - 4]; }
            if (i >= 5) { val += cache[i - 5]; val += cache[i - 5]; }

            /* Negative contributions: -a[i-6] - 2*a[i-7] - 3*a[i-8] - 2*a[i-9] - a[i-10] */
            if (i >= 6) { val -= cache[i - 6]; }
            if (i >= 7) { val -= cache[i - 7]; val -= cache[i - 7]; }
            if (i >= 8) { val -= cache[i - 8]; val -= cache[i - 8]; val -= cache[i - 8]; }
            if (i >= 9) { val -= cache[i - 9]; val -= cache[i - 9]; }
            if (i >= 10){ val -= cache[i - 10]; }

            cache[i] = val;
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
