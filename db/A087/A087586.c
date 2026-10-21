/*
 * A087586  a(1) = 1, a(n) = smallest square beginning with the digit reversal
 *          of a(n-1).
 * https://oeis.org/A087586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1 a(n) = smallest square beginning with the
 *            decimal-reversal of a(n-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/reverse_digits.h"

static seq_t next_term(seq_t revprev, seq_t prev)
{
    seq_t pow10k = 1;          /* 10^k */

    while (1) {
        seq_t start = revprev * pow10k;                     /* revprev*10^k */
        seq_t end   = (revprev + 1) * pow10k - 1;           /* (revprev+1)*10^k - 1 */

        /* If this block ends before the first number greater than prev,
           increase k. */
        if (end < prev + 1) {
            pow10k = pow10k * 10;
            continue;
        }

        /* Threshold = max(prev+1, start). */
        seq_t threshold = prev + 1;
        if (threshold < start)
            threshold = start;

        /* ceil(sqrt(threshold)) */
        seq_t s = floor_sqrt(threshold);
        if (s * s < threshold)
            s = s + 1;

        seq_t square = s * s;
        if (square <= end)
            return square;

        pow10k = pow10k * 10;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* initialise a(1) */
    if (cache_size == 0) {
        cache_cap   = 1;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = 1;            /* a(1) */
        cache_size  = 1;
    }

    /* grow cache up to index n */
    if ((size_t)n > cache_size) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;          /* double */

        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_size; i < (size_t)n; ++i) {
            seq_t prev     = cache[i - 1];
            seq_t revprev  = reverse_digits(prev);
            cache[i]      = next_term(revprev, prev);
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
