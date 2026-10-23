/*
 * A031491  Numbers whose base-8 representation has 2 fewer 0's than 7's.
 * https://oeis.org/A031491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-8 representation has 2 fewer 0's than 7's.
   a(n) is the n-th positive integer such that
   (# of digits equal to 7 in base 8) – (# of digits equal to 0 in base 8) = 2.
   The sequence starts 63, 127, 191, 255, 319, … */

#include <stdint.h>
#include <stdbool.h>

#define MAX_CACHE 20000                     /* enough for n up to ~10000 */

static idx_t  next_check = 1;              /* next integer to test       */
static seq_t  cache[MAX_CACHE];           /* cached values              */
static idx_t  cache_len = 0;              /* number of cached values    */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 1 */
    if (n == 0) return 0;

    /* return cached value if already computed */
    if (n <= cache_len) return cache[n - 1];

    idx_t found = cache_len;
    seq_t result = 0;

    /* search for the next numbers fulfilling the condition */
    while (found < n)
    {
        seq_t val = (seq_t)next_check;
        idx_t zeros  = 0, sevens = 0;
        seq_t tmp = val;

        /* count 0’s and 7’s in the octal representation of val */
        while (tmp > 0)
        {
            unsigned digit = (unsigned)(tmp % 8);
            if (digit == 0) zeros++;
            else if (digit == 7) sevens++;
            tmp /= 8;
        }

        if (sevens == zeros + 2)
        {
            found++;
            if (found <= MAX_CACHE) cache[found - 1] = val;

            if (found == n)
            {
                result = val;    /* last needed value */
                break;
            }
        }
        next_check++;     /* advance to the next candidate */
    }

    /* keep the cache length up to MAX_CACHE for future calls */
    if (found <= MAX_CACHE)
        cache_len = found;
    else
        cache_len = MAX_CACHE;

    /* after finding the n-th term, advance next_check to continue future
       searches from the next integer. */
    next_check++;

    return result;
}
