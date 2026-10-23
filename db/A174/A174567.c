/*
 * A174567  Numbers n such that sum of squares of digits of n is a power of 2.
 * https://oeis.org/A174567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that sum of squares of decimal digits of n is a power of two (OEIS A174567). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_power_of_two.h"

/* Fixed maximum number of sequence elements to cache.  This should be
   large enough for all tests (≈1 000 000 is sufficient for n up to 10 000). */
#ifndef MAX_CACHE
#define MAX_CACHE 1000000
#endif

static seq_t cache[MAX_CACHE];   /* cached terms */
static size_t count = 0;          /* number of terms stored */
static seq_t next_num = 1;        /* candidate number to test */

/* Return true if num has the property: the sum of squares of its decimal
   digits is a power of two. */
static bool good_number(seq_t num)
{
    seq_t sum = 0;
    seq_t t   = num;
    while (t > 0)
    {
        seq_t d = t % 10;
        sum += d * d;
        t   /= 10;
    }
    return is_power_of_two(sum);
}

/* Compute terms up to index n_idx (1‑based). */
static void compute_up_to(idx_t n_idx)
{
    while (count < n_idx)
    {
        if (good_number(next_num))
        {
            if (count == MAX_CACHE)
            {
                /* Should not happen for the intended use. */
                abort();
            }
            cache[count++] = next_num;
        }
        next_num++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe */
    compute_up_to(n);
    return cache[n - 1];
}
