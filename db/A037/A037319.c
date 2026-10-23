/*
 * A037319  Numbers whose base-4 and base-8 expansions have the same digit
 *          sum.
 * https://oeis.org/A037319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation:
   a(n) is the nth positive integer whose base‑4 and base‑8 digit sums are equal. */
#include <stdlib.h>
#include <stdint.h>

static int digit_sum(seq_t x, int base)
{
    int sum = 0;
    while (x != 0)
    {
        sum += (int)(x % base);
        x /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* array of found terms                */
    static size_t cache_size = 0;      /* number of terms currently stored   */
    static size_t cache_cap  = 0;      /* allocated capacity of cache        */
    static seq_t next_candidate = 1;   /* next integer to test              */

    if (n == 0) return 0;              /* undefined for offset 1, guard      */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        int sum4 = digit_sum(next_candidate, 4);
        int sum8 = digit_sum(next_candidate, 8);

        if (sum4 == sum8)
        {
            if (cache_size >= cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                /* cast is required to make this code compile as both C and C++ */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
