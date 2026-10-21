/*
 * A219408  Unmatched value maps: number of n X 6 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..2 n X 6 array.
 * https://oeis.org/A219408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 21*a(n-1) + 950*a(n-2) + 9306*a(n-3) + 26031*a(n-4)
   - 50016*a(n-5) - 214765*a(n-6) + 276749*a(n-7) + 349461*a(n-8)
   - 523220*a(n-9) - 71125*a(n-10) + 294741*a(n-11) - 112428*a(n-12)
   + 11664*a(n-13)   for n > 14
   Initial values:
     a(1)  = 21
     a(2)  = 1261
     a(3)  = 57097
     a(4)  = 2633596
     a(5)  = 121641579
     a(6)  = 5618116265
     a(7)  = 259468310384
     a(8)  = 11983486642214
     a(9)  = 553453107750115
     a(10) = 25561051456876291
     a(11) = 1180528707338785094
     a(12) = 54522328912450097473
     a(13) = 2518095766994664668337
     a(14) = 116297422017630310505820
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

static const char *const init_strs[14] = {
    "21",                                 /* a(1) */
    "1261",                               /* a(2) */
    "57097",                              /* a(3) */
    "2633596",                            /* a(4) */
    "121641579",                          /* a(5) */
    "5618116265",                         /* a(6) */
    "259468310384",                       /* a(7) */
    "11983486642214",                     /* a(8) */
    "553453107750115",                    /* a(9) */
    "25561051456876291",                  /* a(10) */
    "1180528707338785094",                /* a(11) */
    "54522328912450097473",               /* a(12) */
    "2518095766994664668337",             /* a(13) */
    "116297422017630310505820"            /* a(14) */
};

static seq_t *cache = NULL;   /* cache[0] holds a(1) */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* Expand cache to hold at least 'n' terms (n >= cache_size + 1) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

/* Returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, but harmless */

    if ((size_t)n <= cache_size)
        return cache[n - 1];

    ensure_capacity(n);

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        if ((size_t)(i - 1) < 14)
        {
            /* Initialize from the stored string literals */
            cache[i - 1] = seq_from_str(init_strs[i - 1]);
        }
        else
        {
            /* Apply the recurrence */
            seq_t sum = 0;

            sum += (seq_t)21      * cache[i - 2];
            sum += (seq_t)950     * cache[i - 3];
            sum += (seq_t)9306    * cache[i - 4];
            sum += (seq_t)26031   * cache[i - 5];
            sum -= (seq_t)50016   * cache[i - 6];
            sum -= (seq_t)214765  * cache[i - 7];
            sum += (seq_t)276749  * cache[i - 8];
            sum += (seq_t)349461  * cache[i - 9];
            sum -= (seq_t)523220  * cache[i -10];
            sum -= (seq_t)71125   * cache[i -11];
            sum += (seq_t)294741  * cache[i -12];
            sum -= (seq_t)112428  * cache[i -13];
            sum += (seq_t)11664   * cache[i -14];

            cache[i - 1] = sum;
        }
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
