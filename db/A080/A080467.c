/*
 * A080467  Multiples of 11 in which the even positioned digits from left are
 *          odd and the odd positioned ones are even.
 * https://oeis.org/A080467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for multiples of 11 whose digits alternate parity:
   odd‑position digits are even, even‑position digits are odd.  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* last filled index (1‑based) */
    static size_t cache_cap = 0;
    static seq_t next_multiple = 11;

    /* initialise cache with a(1) = 0 */
    if (cache_len == 0)
    {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = 0;     /* dummy to keep 1‑based indexing */
        cache[1] = 0;     /* a(1) */
        cache_len = 1;
    }

    /* generate terms until we have the desired index */
    while (cache_len < n)
    {
        seq_t m = next_multiple;
        next_multiple += 11;   /* next candidate multiple of 11 */

        /* check digit–parity pattern */
        seq_t tmp = m;
        int digits[64];
        int len = 0;
        while (tmp > 0)
        {
            digits[len++] = (int)(tmp % 10);
            tmp /= 10;
        }

        bool good = true;
        for (int pos = 1; pos <= len; ++pos)
        {
            int d = digits[len - pos];   /* most significant digit at pos 1 */
            if (pos & 1)          /* odd position → even digit */
            {
                if (d & 1) { good = false; break; }
            }
            else                  /* even position → odd digit */
            {
                if ((d & 1) == 0) { good = false; break; }
            }
        }

        if (good)
        {
            /* ensure sufficient capacity */
            if (cache_len + 1 >= cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 2;
                while (new_cap <= cache_len + 1) new_cap <<= 1;
                seq_t *tmp_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp_arr;
                cache_cap = new_cap;
            }
            cache_len++;
            cache[cache_len] = m;
        }
    }

    return cache[n];
}
