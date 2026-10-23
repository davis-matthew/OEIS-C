/*
 * A034087  Numbers divisible by the sum of the squares of their digits.
 * https://oeis.org/A034087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034087: Numbers divisible by the sum of the squares of their decimal digits.
   The function builds the sequence lazily, storing found terms in a static array so
   that any n >= 1 can be answered in O(1) after the needed terms have been generated.*/

#include <stdlib.h>
#include <stdbool.h>

/* Cache large enough for typical use (thousands of terms). */
#define CACHE_LIMIT 200000

static seq_t cache[CACHE_LIMIT];
static size_t cache_size = 0;          /* number of terms stored so far */
static seq_t next_candidate = 1;      /* next integer to test */

/* Compute sum of squares of decimal digits of x. */
static seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        sum += (seq_t)(d * d);
        x /= 10;
    }
    return sum;
}

extern seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        seq_t sum_sq = digit_square_sum(next_candidate);

        if (sum_sq != 0 && (next_candidate % sum_sq) == 0)
        {
            if (cache_size >= CACHE_LIMIT) /* defensive check */
                abort();                  /* should not happen for normal use */

            cache[cache_size++] = next_candidate;
        }

        ++next_candidate;
    }

    /* Return the nth term (1‑based index). */
    return cache[(size_t)n - 1];
}
