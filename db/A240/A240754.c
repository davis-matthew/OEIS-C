/*
 * A240754  Numbers k with digit_sum(k*k) - 1 = digit_sum((k+1)*(k+1)).
 * https://oeis.org/A240754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 2785, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) are the integers k such that digit_sum(k*k)-1 == digit_sum((k+1)*(k+1)) */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache of found terms – allows arbitrary order of calls */
    static seq_t *terms = NULL;    /* array of found k values          */
    static size_t cap   = 0;       /* allocated capacity in elements   */
    static size_t sz    = 0;       /* number of stored terms           */
    static idx_t  last  = 0;       /* last integer k examined          */

    if (n == 0)
        return 0;                  /* offset is 1, undefined for 0    */

    /* Generate terms until we have n of them */
    while (sz < n) {
        idx_t k = ++last;                     /* candidate integer to test   */
        seq_t v = (seq_t)k;                   /* cast to seq_t for arithmetic */

        seq_t k2  = v * v;                    /* k * k                        */
        seq_t kp1 = v + 1;                    /* k + 1                        */
        seq_t kp12 = kp1 * kp1;               /* (k + 1)^2                    */

        /* If the digit‑sum condition holds, store the term */
        if (digit_sum(k2) - (seq_t)1 == digit_sum(kp12)) {
            if (sz == cap) {                 /* grow cache if needed       */
                size_t newcap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) abort();           /* out of memory              */
                terms = tmp;
                cap   = newcap;
            }
            terms[sz++] = v;
        }
    }

    return terms[n - 1];
}
