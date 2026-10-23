/*
 * A041667  Denominators of continued fraction convergents to sqrt(352).
 * https://oeis.org/A041667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041667: Denominators of continued fraction convergents to sqrt(352)
   Formula: a(n) = 155234*a(n-8) - a(n-16) for n>15
   Initial values (n=0..15):
       1, 1, 4, 21, 193, 986, 3151, 4137,
       152083, 156220, 620743, 3259935,
       29960158, 153060725, 489142333, 642203058
   This implementation caches the computed terms and extends the cache
   lazily.  It works for any build that supplies seq_t and idx_t. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Table of the first 16 terms */
    static const seq_t init[16] = {
        (seq_t)1, (seq_t)1, (seq_t)4,       (seq_t)21,
        (seq_t)193, (seq_t)986, (seq_t)3151, (seq_t)4137,
        (seq_t)152083, (seq_t)156220, (seq_t)620743,
        (seq_t)3259935, (seq_t)29960158, (seq_t)153060725,
        (seq_t)489142333, (seq_t)642203058
    };

    /* Maximum index expected by the tests.  The problem statement
       suggests n ≤ ~10000.  For larger indices the array would need
       to be enlarged. */
    enum { MAX_N = 10000 };
    static seq_t cache[MAX_N + 1];
    static bool initialized = false;
    static size_t last = 0;   /* highest index that has a valid value */

    /* Initialise the cache with the first 16 terms */
    if (!initialized) {
        for (size_t i = 0; i < 16; ++i) cache[i] = init[i];
        last = 15;
        initialized = true;
    }

    /* Extend the cache up to the requested index, if necessary */
    if ((size_t)n > last) {
        /* The loop runs from last+1 through n inclusively */
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            /* For i < 16 the value is already set above */
            cache[i] = (seq_t)155234 * cache[i - 8] - cache[i - 16];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
