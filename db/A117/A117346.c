/*
 * A117346  Near-multiperfects: numbers m such that abs(sigma(m) mod m) <=
 *          log(m).
 * https://oeis.org/A117346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m (n≥1) such that min( sigma(m)
 *            mod m , m‑(sigma(m) mod m) ) ≤ ⌊log(m)⌋ where sigma(m) is the
 *            sum of all positive divisors of m. The function builds the
 *            sequence on demand with a static cache. The condition follows
 *            the OEIS definition via the minimum distance from...
 * Range:     u8 n <= 43, u16 n <= 4381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <math.h>     /* log(double) */

#include "../utils/sigma_of.h"
#include "../utils/grow_seq_array.h"

/* Static cache of discovered sequence terms. */
static seq_t *seq_cache    = NULL;   /* dynamically grown array of terms   */
static size_t cache_size   = 0;      /* number of elements stored          */
static size_t cache_cap    = 0;      /* current allocation capacity        */
static idx_t   last_tested = 0;      /* largest integer examined so far    */

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)                /* the sequence is 1‑based; 0 is unused */
        return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n) {
        /* Next integer to test. */
        last_tested++;

        /* Compute σ(last_tested). */
        seq_t sigma = sigma_of(last_tested);

        /* Remainder modulo last_tested (0 ≤ rem < last_tested). */
        seq_t rem_seq = sigma % (seq_t)last_tested;
        uint64_t rem   = (uint64_t)rem_seq;              /* fits in uint64_t */

        /* Distance to the nearest multiple of last_tested. */
        uint64_t diff = rem;
        uint64_t alt  = last_tested - rem;
        if (alt < diff) diff = alt;                      /* min(rem, m‑rem) */

        /* Floor of the natural logarithm of last_tested. */
        double logd      = log((double)last_tested);
        uint64_t floor_log = (uint64_t)logd;             /* truncates → floor */

        /* Accept if the distance is within the logarithmic bound. */
        if (diff <= floor_log) {
            /* Ensure capacity for one more term. */
            if (cache_size + 1 > cache_cap)
                grow_seq_array(&seq_cache, &cache_cap, cache_size + 1);

            seq_cache[cache_size++] = last_tested;
        }
    }

    /* Return the desired n‑th term (indices are 1‑based). */
    return seq_cache[n - 1];
}
