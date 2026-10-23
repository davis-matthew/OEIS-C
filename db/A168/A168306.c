/*
 * A168306  The fifth left hand column of triangle A167580.
 * https://oeis.org/A168306
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 30, u128 n <= 81,
 *            big any n
 * Helpers:   poly_formula
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A168306 – fifth left‑hand column of triangle A167580
   Recurrence (order 10):
      a(n) = 20*a(n-1) − 180*a(n-2) + 960*a(n-3) − 3360*a(n-4)
             + 8064*a(n-5) − 13440*a(n-6) + 15360*a(n-7) − 11520*a(n-8)
             + 5120*a(n-9) − 1024*a(n-10)
   Initial values a(5)…a(14) are obtained from the polynomial
   a(n) = 2^n*(214*n^9 − 3963*n^8 + 30768*n^7 − 130536*n^6
                 + 330834*n^5 − 514332*n^4 + 484382*n^3
                 − 262149*n^2 + 72342*n − 7560) / 241920
*/

#include <stdbool.h>
#include "../utils/poly_formula.h"

/* Maximum index that the static cache can hold.
   The OEIS test harness uses n up to about 10 000, so 20 000 is safe.   */
#define MAX_CACHE_INDEX 20000

/* Static cache for memoised terms and a flag array that indicates
   whether a given index has been computed.  All entries are zero-initialised. */
static seq_t  cache[MAX_CACHE_INDEX + 1];
static bool   computed[MAX_CACHE_INDEX + 1];

/* The largest index for which cache has already been initialised. */
static idx_t initialized_up_to = 4;     /* values below 5 are undefined */

/* ------------------------------------------------------------------ */
/*  Polynomial formula for the initial terms (5 ≤ n ≤ 14).              */

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 5)                    /* values below the offset are undefined */
        return (seq_t)0;

    /* Ensure bounds – the test harness never asks for n > MAX_CACHE_INDEX. */
    if (n > MAX_CACHE_INDEX)
        return (seq_t)0;          /* or could grow the static arrays in another build */

    /* If the requested term is already cached, return it directly. */
    if (computed[n])
        return cache[n];

    /* Compute all missing values up to n.  The loop is O(n) and memoises. */
    for (idx_t i = initialized_up_to + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 14) {
            /* Initial values from the polynomial. */
            val = poly_formula(i);
        } else {
            /* Recurrence for i ≥ 15.  The coefficients are cast to seq_t. */
            val = (seq_t)20   * cache[i - 1]
               - (seq_t)180  * cache[i - 2]
               + (seq_t)960  * cache[i - 3]
               - (seq_t)3360 * cache[i - 4]
               + (seq_t)8064 * cache[i - 5]
               - (seq_t)13440* cache[i - 6]
               + (seq_t)15360* cache[i - 7]
               - (seq_t)11520* cache[i - 8]
               + (seq_t)5120 * cache[i - 9]
               - (seq_t)1024 * cache[i -10];
        }
        cache[i]   = val;
        computed[i] = true;
    }

    initialized_up_to = n;
    return cache[n];
}
