/*
 * A102112  Iccanobirt numbers (2 of 15): a(n) = a(n-1) + R(a(n-2)) + a(n-3),
 *          where R is the digit reversal function A004086.
 * https://oeis.org/A102112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + R(a(n-2)) + a(n-3) for n >= 4 Initial values:
 *            a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 1 where R(x) is the decimal
 *            digit reversal of x
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 30, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Reverse the decimal digits of a seq_t value */
static inline seq_t reverse_seq(seq_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of entries currently cached (next free index) */

    /* enlarge cache if necessary */
    if (n >= cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)   /* allocation failed – leave original cache untouched */
            return 0; /* undefined behaviour; caller shouldn't call with huge n if no memory */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
                cache[i] = 0;
            else if (i == 1)
                cache[i] = 0;
            else if (i == 2)
                cache[i] = 1;
            else if (i == 3)
                cache[i] = 1;
            else
            {
                seq_t r = reverse_seq(cache[i - 2]);          /* R(a(i-2)) */
                cache[i] = cache[i - 1] + r + cache[i - 3];
            }
        }

        cache_size = new_size;
    }

    return cache[n];
}
