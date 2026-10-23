/*
 * A144205  Fibonacci numbers containing an equal number of odd and even
 *          digits.
 * https://oeis.org/A144205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   digit_counts, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence a(n) of Fibonacci numbers that contain an equal
   number of odd and even digits.  The function generates Fibonacci
   numbers until enough satisfy the digit condition.  The implementation
   works for all types supported by the build: unsigned integer types,
   __int128, or an arbitrary‑precision integer class.  All memory is
   managed via malloc/realloc; a cast from void* to seq_t* is included
   to satisfy both C and C++ compilers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Provided utilities */
#include "../utils/digit_counts.h"   /* void digit_counts(seq_t x, unsigned char cnt[10]) */
#include "../utils/seq_one.h"        /* seq_t seq_one(void) */

/* Check whether x has the same number of odd as even digits. */
static inline bool has_equal_odd_even_digits(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    int odd  = cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9];
    int even = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
    return odd == even;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *found = NULL;          /* array of qualifying Fibs                */
    static size_t found_len = 0;         /* number already stored                    */
    static size_t found_cap = 0;         /* allocated capacity                       */
    static seq_t prev, curr;             /* current and next Fibonacci numbers       */
    static bool init = false;            /* one‑time initialization flag             */

    if (!init)
    {
        prev = seq_one();                /* F(1) = 1                               */
        curr = seq_one();                /* F(2) = 1                               */
        init = true;
    }

    /* Grow the cache until it contains at least n valid terms. */
    while (found_len < (size_t)n)
    {
        /* Check the current Fibonacci number (prev). */
        if (has_equal_odd_even_digits(prev))
        {
            if (found_len == found_cap)
            {
                size_t new_cap = found_cap ? found_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
                if (!tmp) abort();       /* memory exhausted */
                found     = tmp;
                found_cap = new_cap;
            }
            found[found_len++] = prev;
        }

        /* Advance to the next Fibonacci number. */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    /* Return the requested term. */
    return found[(size_t)(n - 1)];
}
