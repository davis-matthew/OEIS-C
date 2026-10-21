/*
 * A077698  a(1) = 3, a(n) is the smallest multiple of a(n-1) that contains
 *          all the digits of a(n-1), or which can be obtained by inserting
 *          digits anywhere in a permutation of digits of a(n-1) (prefix,
 *          suffix or insertion). Zeros are not to be suffixed. a(n) is not
 *          divisible by 10.
 * https://oeis.org/A077698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3; for n>1, a(n) is the smallest multiple of a(n−1)
   greater than a(n−1) that is not divisible by 10 and whose decimal digits
   contain all the digits of a(n−1) (each at least once).  The function
   memoises already computed terms so that any call with any n ≥ 1
   (in any order) returns the correct value.  It works for all provided
   seq_t/idx_t types. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

/* return a(n) for n ≥ 1; a(0) is undefined and returns 0 */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *terms   = NULL;  /* 0‑based array: terms[0] = a(1) */
    static idx_t  last    = 0;     /* number of cached terms (≥1 after init) */
    static size_t cap     = 0;     /* current allocation size */

    if (n == 0)
        return 0;   /* undefined, but safe */

    /* lazy initialisation */
    if (last == 0) {
        cap   = 1;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms)
            return 0;                 /* allocation failure – unlikely in tests */
        terms[0] = (seq_t)3;           /* a(1) */
        last    = 1;
    }

    /* compute terms up to n if required */
    while (last < n) {
        seq_t prev = terms[last - 1];

        /* digit frequencies of previous term */
        unsigned int prevfreq[10] = {0};
        digit_frequencies(prev, prevfreq);

        /* search for the smallest valid multiple > prev */
        seq_t factor = (seq_t)2;
        seq_t candidate;
        for (;;) {
            candidate = prev * factor;

            /* reject multiples ending in 0 */
            if (candidate % (seq_t)10 != 0) {
                unsigned int curfreq[10] = {0};
                digit_frequencies(candidate, curfreq);

                bool ok = true;
                for (int d = 0; d < 10 && ok; ++d)
                    if (curfreq[d] < prevfreq[d])
                        ok = false;

                if (ok)
                    break;   /* candidate found */
            }
            ++factor;   /* try next multiple */
        }

        /* resize cache if necessary */
        if (last >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            if (!tmp)   /* allocation failure – return what we have */
                return terms[last - 1];
            terms = tmp;
        }

        terms[last++] = candidate;
    }

    return terms[n - 1];
}
