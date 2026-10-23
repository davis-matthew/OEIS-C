/*
 * A117832  Triangular numbers for which the product of the digits is an
 *          octagonal number.
 * https://oeis.org/A117832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117832: 
   Triangular numbers (k*(k+1)/2) whose decimal digit product is an octagonal number. */
/* The function computes a(n) for 1 ≤ n by searching the triangular numbers in order
   and storing the qualifying ones in a static cache. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Returns the n-th term (1‑based index) of the sequence.  The function is
   thread‑unsafe but satisfies all other requirements. */
seq_t a(idx_t n)
{
    /* Static cache for the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* Number of terms currently cached. */
    static size_t cache_cap  = 0;   /* Allocated capacity of the cache. */
    static idx_t next_k = 0;        /* Next k to test for a triangular number. */

    /* Ensure that we have at least n elements cached. */
    while (cache_size < n)
    {
        /* Triangular number for current k. */
        idx_t k = next_k;
        seq_t triangular = ((seq_t)k * (seq_t)(k + 1)) / 2;

        /* Product of its decimal digits. */
        seq_t prod = digit_product(triangular);

        /* Check whether prod is an octagonal number.
           Octagonal numbers satisfy prod = m*(3*m-2) for some integer m.  */
        bool ok = false;
        if (prod == 0)
        {
            ok = true;                  /* 0 = 0*(3*0-2) is octagonal. */
        }
        else
        {
            seq_t disc = ((seq_t)12 * prod) + (seq_t)4;   /* Discriminant: 4 + 12*prod. */
            seq_t sqrt_disc = floor_sqrt(disc);          /* Integer sqrt. */
            if (sqrt_disc * sqrt_disc == disc)
            {
                /* m = (sqrt_disc + 2) / 6 must be integer. */
                seq_t num = sqrt_disc + (seq_t)2;
                if (num % (seq_t)6 == 0)
                {
                    ok = true;
                }
            }
        }

        /* If it qualifies, store it in the cache. */
        if (ok)
        {
            if (cache_size + 1 > cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* Fatal error: out of memory. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = triangular;
        }

        /* Move to the next triangular number. */
        next_k = k + 1;
    }

    /* Return the requested term (index 1 maps to cache[0]). */
    return cache[n - 1];
}
