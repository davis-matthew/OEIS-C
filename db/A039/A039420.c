/*
 * A039420  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 7's.
 * https://oeis.org/A039420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a brute‑force search over the integers, selecting those numbers whose decimal
   representation contains the same nonzero number of digits 3 and 7. */

#include <stdlib.h>
#include <stdbool.h>

/*--- Static cache to allow non‑increasing calls -------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated capacity */
static idx_t stored = 0;        /* number of terms currently stored */
static seq_t next_start = 37;   /* next integer to test */

/*--- Helper: test whether a number satisfies the condition ---------------------------*/
static bool matches(seq_t x)
{
    seq_t temp = x;
    int cnt3 = 0;
    int cnt7 = 0;
    while (temp > 0)
    {
        int d = (int)(temp % 10);
        if (d == 3) ++cnt3;
        else if (d == 7) ++cnt7;
        temp = temp / 10;
    }
    return (cnt3 > 0) && (cnt3 == cnt7);
}

/*--- Main sequence function ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; behaviour below is irrelevant */

    if (n > stored)
    {
        /* Ensure enough space in the cache */
        size_t new_cap = (size_t)n;
        if (new_cap > cache_size)
        {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_size = new_cap;
        }

        /* Generate terms until we reach the requested index */
        while (stored < n)
        {
            seq_t val = next_start;
            while (true)
            {
                if (matches(val))
                {
                    cache[stored] = val;
                    next_start = val + 1;
                    ++stored;
                    break;
                }
                val = val + 1;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
