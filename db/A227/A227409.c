/*
 * A227409  Prime numbers representing a date in "condensed American notation"
 *          MMDDYY.
 * https://oeis.org/A227409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1754, u32 n <= 1845, u64 n <= 3379,
 *            u128 n <= 3379, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A227409: prime dates MMDDYY (condensed).
 *  The sequence consists of all primes n of the form
 *
 *      n = month*10000 + day*100 + year
 *
 *  where month ∈ [1,12], day is a valid day number for that
 *  month (Feb has 28 days), and year ∈ [0,99].  The integers are
 *  interpreted in “condensed American notation”, i.e. leading zeros
 *  are omitted.  The terms are listed in increasing numerical order.
 *
 *  The implementation generates all such dates once, keeps the
 *  primes in a static array, and returns the nth term on request.
 *  The algorithm is O(#dates) which is < 12*31*100 = 37200
 *  (only 37200 candidates, 3379 of which are prime).  All operations
 *  use only integer arithmetic and standard library functions.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* static cache of the sequence values */
static seq_t *cache = NULL;
static size_t  cache_used = 0;
static size_t  cache_cap  = 0;

/* initialize the cache with all terms of the sequence */
static void init_cache(void) {
    if (cache != NULL) return;

    /* at most 3379 terms – allocate a bit more for safety */
    cache_cap = 4000;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);          /* out of memory – abort */

    const int days_per_month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    size_t count = 0;
    for (int month = 1; month <= 12; ++month) {
        int maxd = days_per_month[month];
        for (int day = 1; day <= maxd; ++day) {
            for (int year = 0; year < 100; ++year) {
                /* build the condensed date value */
                seq_t val = (seq_t)month * 10000
                          + (seq_t)day   * 100
                          + (seq_t)year;
                if (is_prime_big(val)) {
                    if (count >= cache_cap) {
                        /* reallocate if needed (unlikely) */
                        cache_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                 cache_cap * sizeof(seq_t));
                        if (!tmp) { free(cache); exit(1); }
                        cache = tmp;
                    }
                    cache[count++] = val;
                }
            }
        }
    }
    cache_used = count;   /* number of terms actually stored */
}

/* ------------------------------------------------------------------ */
/*  public function                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for n<1 */

    init_cache();

    size_t pos = (size_t)(n - 1);
    if (pos >= cache_used) {
        /* n is beyond the finite sequence – return 0 as error */
        return 0;
    }
    return cache[pos];
}
