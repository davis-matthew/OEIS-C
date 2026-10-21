/*
 * A067529  n - 5^k is a prime for all k > 0 and n > 5^k.
 * https://oeis.org/A067529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067529:
   a(n) = the nth integer n such that for every k > 0 with 5^k < n,
          the difference n - 5^k is prime. */

/* The function a(idx_t n) returns the nth term (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities provided by the framework */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Static array to store computed terms */
    static seq_t *seq = NULL;      /* array of sequence values */
    static size_t seq_len = 0;     /* number of terms stored */
    static size_t seq_cap = 0;     /* current capacity of the array */

    /* Ensure we have at least n terms computed */
    if (seq_len < (size_t)n)
    {
        /* Start searching from last computed term + 1, or from 6 */
        idx_t current = (seq_len > 0) ? (idx_t)seq[seq_len - 1] + 1 : 6;

        while (seq_len < (size_t)n)
        {
            bool good = true;

            /* Check all powers of 5 that are strictly less than current */
            for (idx_t p = 5; p < current; p *= 5)
            {
                idx_t diff = current - p;
                if (!is_prime(diff))
                {
                    good = false;
                    break;
                }
            }

            if (good)
            {
                /* Store this term */
                if (seq_len >= seq_cap)
                {
                    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)current;
            }

            ++current;
        }
    }

    /* Return the requested term (1‑based index) */
    return seq[n - 1];
}
