/*
 * A044687  Numbers n such that string 6,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044687:
   Numbers n such that the base‑9 representation of n contains the
   consecutive digits 6,1, while the base‑9 representation of n+1
   does not.  The function generates the sequence by scanning integers
   and testing the property.  The result is memoised so that any order
   of calls is efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if the radix‑9 representation of x contains the
   pattern 6 followed by 1. */
static bool has61(idx_t x)
{
    if (x == (idx_t)0)
        return false;

    /* 9^20 < 2^64, so at most 20 base‑9 digits are required for idx_t. */
    unsigned char digits[20];
    size_t len = 0;

    while (x > (idx_t)0)
    {
        digits[len++] = (unsigned char)(x % 9);
        x /= (idx_t)9;
    }

    /* Look for 6 (higher) followed by 1 (lower). */
    for (size_t i = len; i-- > 1; )
    {
        if (digits[i] == 6 && digits[i-1] == 1)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Undefined for n==0 – return 0 as a harmless default. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Static memoisation buffer. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  scan_from  = 1;   /* Next integer to test. */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        /* Find the next qualifying number. */
        while (1)
        {
            idx_t candidate = scan_from++;

            if (has61(candidate) && !has61(candidate + 1))
            {
                /* Ensure we have room in the buffer. */
                if (cache_size >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();               /* Out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)candidate;
                break;   /* Move on to the next requested term. */
            }
        }
    }

    return cache[n - 1];
}
