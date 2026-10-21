/*
 * A064209  Numbers k whose sum of digits exceeds the sum of the digits of
 *          k^3.
 * https://oeis.org/A064209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k whose sum of digits exceeds the sum of the digits of k^3 */
/* This implementation simply searches upward for successive k satisfying
   sum_digits(k) > sum_digits(k^3), caching found values for fast reuse. */

#include <stdint.h>
#include <stdlib.h>

/* Utility: sum of decimal digits of a 64‑bit integer */
static unsigned int digit_sum_uint64(uint64_t x)
{
    unsigned int sum = 0;
    while (x)
    {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Utility: sum of decimal digits of an unsigned __int128 integer */
static unsigned int digit_sum_uint128(unsigned __int128 x)
{
    unsigned int sum = 0;
    while (x)
    {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static memoisation of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;          /* number of terms stored */
    static uint64_t last_k = 0;           /* last integer checked */

    /* Initialise the storage on first call */
    if (terms_cap == 0)
    {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n)
    {
        /* Look for the next integer that satisfies the condition */
        for (;;)
        {
            last_k++;                           /* candidate k */

            unsigned int sum_k = digit_sum_uint64(last_k);

            /* Compute cube in 128‑bit to avoid overflow */
            unsigned __int128 cube = (unsigned __int128)last_k *
                                    (unsigned __int128)last_k *
                                    (unsigned __int128)last_k;

            unsigned int sum_cube = digit_sum_uint128(cube);

            if (sum_k > sum_cube)
                break;                       /* found next term */
        }

        /* Append new term to the array, expanding if necessary */
        if (terms_len == terms_cap)
        {
            terms_cap <<= 1;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_len++] = (seq_t)last_k;
    }

    return terms[n - 1];
}
