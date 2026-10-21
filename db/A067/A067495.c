/*
 * A067495  Powers of 9 having initial digit 1.
 * https://oeis.org/A067495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 9^k such that the decimal representation of 9^k starts
 *            with 1 (OEIS A067495. a(1) = 1, a(2) = 1853020188851841, … ).
 *            The implementation computes the terms by iterating powers of 9,
 *            checks the leading digit using the fractional part of k *
 *            log10(9), and caches the results for fast repeated...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>          /* for fmodl */
#include "../utils/seq_mul.h"

static const long double LOG10_9 = 0.9542425094393249L;
static const long double LOG10_2 = 0.3010299956639812L;

seq_t a(idx_t n)
{
    /* Cache of terms that satisfy the condition */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Current power of 9 and associated data */
    static seq_t current = 0;   /* 9^k */
    static idx_t  next_exp = 0; /* exponent of current */
    static long double frac = 0.0L; /* fractional part of k*log10(9) */

    /* Undefined for index 0; return 0 to avoid UB */
    if (n == 0)
        return 0;

    /* Initialise static data on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();            /* allocation failure */
        current   = (seq_t)1; /* 9^0 */
        cache[0]  = current;
        cache_size = 1;
        next_exp   = 0;
        frac       = 0.0L;
    }

    /* Generate terms until we have at least n cached */
    while (cache_size < (size_t)n) {
        /* Advance to next exponent */
        next_exp++;

        /* current = current * 9 */
        current = seq_mul(current, (seq_t)9);

        /* Update fractional part: (k+1)*log10(9) = k*log10(9) + log10(9) */
        frac += LOG10_9;
        if (frac >= 1.0L)
            frac -= 1.0L;   /* keep in [0,1) */

        /* Leading digit 1 iff fractional part < log10(2) */
        if (frac < LOG10_2) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current;
        }
    }

    return cache[n - 1];
}
