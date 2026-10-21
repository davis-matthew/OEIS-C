/*
 * A239590  Numbers whose cubes are cyclops numbers.
 * https://oeis.org/A239590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 35, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the nth term of OEIS A239590.
   A number m is in the sequence iff its cube m³ is a cyclops number:
   the decimal representation of m³ has an odd number of digits, contains
   exactly one zero, and that zero occupies the middle position.
*/

/* Include the standard header needed for realloc */
#include <stdlib.h>

static seq_t *cache = NULL;     /* stored sequence values */
static size_t cache_len = 0;    /* number of values currently cached */
static size_t cache_cap = 0;    /* allocated capacity of cache array */
static idx_t last_tested = 0;   /* next integer to test for inclusion */

/* Return the nth term (1‑indexed) of A239590 */
seq_t a(idx_t n)
{
    /* Return already cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* Compute the cube of the candidate integer */
        seq_t cube = (seq_t)last_tested * (seq_t)last_tested * (seq_t)last_tested;

        /* Count digits, zero occurrences and position of the zero digit */
        idx_t len = 0;           /* digit count of the cube */
        idx_t zero_pos = 0;      /* position of the zero digit (LSB = 0) */
        idx_t zero_cnt = 0;      /* number of zero digits */

        /* Special handling for cube == 0 */
        if (cube == 0)
        {
            len = 1;
            zero_pos = 0;
            zero_cnt = 1;
        }
        else
        {
            seq_t tmp = cube;
            while (tmp > 0)
            {
                seq_t d = tmp % 10;
                if (d == 0)
                {
                    zero_cnt++;
                    zero_pos = len;
                }
                tmp /= 10;
                len++;
            }
        }

        /* Accept the number iff it satisfies the cyclops condition */
        if (zero_cnt == 1 && (len & 1) && zero_pos == (len >> 1))
        {
            /* Grow the cache array if needed */
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_tested;
        }

        /* Advance to the next candidate integer */
        last_tested++;
    }

    /* The requested term has just been added to the cache */
    return cache[n - 1];
}
