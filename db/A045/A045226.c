/*
 * A045226  Numbers whose base-5 representation contains exactly two 1's and
 *          one 2.
 * https://oeis.org/A045226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045226: numbers whose base‑5 representation contains exactly
   two 1's and one 2.  The function generates the sequence by
   scanning integers and picking those with the required digit
   pattern. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined but harmless */

    /* Static storage for cached terms */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_len = 0;
    static seq_t   last_val  = 0;   /* next integer to test */

    /* Ensure the cache holds at least n entries */
    if (cache_len < n) {
        /* Initialise or grow the cache */
        if (cache == NULL) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        } else if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Scan subsequent integers until the cache is full */
        while (cache_len < n) {
            seq_t val = ++last_val;          /* candidate value */

            /* Count digits 1 and 2 in base‑5 */
            seq_t  temp = val;
            int   c1 = 0, c2 = 0;
            while (temp > 0) {
                seq_t digit = temp % 5;
                if (digit == 1) c1++;
                else if (digit == 2) c2++;
                temp /= 5;
            }

            if (c1 == 2 && c2 == 1) {
                cache[cache_len++] = val;
            }
        }
    }

    return cache[n - 1];
}
