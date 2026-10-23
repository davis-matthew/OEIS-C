/*
 * A130738  Greedy odd Egyptian fraction representation of 1 (without
 *          repeats).
 * https://oeis.org/A130738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) computes the n-th odd Egyptian fraction in the greedy
 *            representation of 1 without repetition. The algorithm keeps the
 *            current sum as a fraction `sum_num/sum_den` and chooses at each
 *            step the largest odd integer `d` not used before that satisfies
 *            `1/d <= 1 - sum_num/sum_den`. The sequence is...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   gcd, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

/* cache of already computed denominators */
static seq_t *den_cache = NULL;
static size_t den_count = 0;   /* number of values stored */
static size_t den_cap   = 0;   /* current capacity */

/* current partial sum of the greedy representation */
static seq_t sum_num = 0;   /* numerator of the partial sum (0 initially) */
static seq_t sum_den = 1;   /* denominator of the partial sum (1 initially) */
static seq_t last_den = 0;  /* last chosen denominator (0 initially) */

/* enlarge the cache to hold at least `n` elements */
static void
ensure_den_cache(size_t n)
{
    if (n <= den_cap)
        return;
    size_t new_cap = den_cap ? den_cap << 1 : 8;
    while (new_cap < n)
        new_cap <<= 1;
    den_cache = seq_realloc(den_cache, den_cap, new_cap);
    den_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1, but avoid UB */

    /* compute as many terms as required, storing them in the cache */
    if (den_count < (size_t)n) {
        ensure_den_cache((size_t)n);

        /* loop until we have at least n values in the cache */
        while (den_count < (size_t)n) {
            /* remaining fraction R = 1 - sum = (sum_den - sum_num)/sum_den */
            seq_t denom_rem = sum_den - sum_num;   /* > 0 */

            /* ceil( sum_den / denom_rem ) */
            seq_t t = sum_den / denom_rem;
            if (sum_den % denom_rem != 0)
                ++t;

            /* candidate denominator must be > last_den, odd, and at least 3 */
            seq_t cand = t;
            if (cand <= last_den)
                cand = last_den + 1;
            if ( (cand & 1) == 0 )
                ++cand;
            if (cand < 3)
                cand = 3;

            /* store chosen denominator */
            den_cache[den_count] = cand;

            /* update the partial sum:  sum += 1/cand
               new_num = sum_num * cand + sum_den
               new_den = sum_den * cand              */
            seq_t new_num = sum_num * cand + sum_den;
            seq_t new_den = sum_den * cand;

            /* reduce the fraction */
            seq_t g = gcd(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;

            last_den = cand;
            ++den_count;
        }
    }

    return den_cache[n - 1];
}
