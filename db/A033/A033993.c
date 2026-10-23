/*
 * A033993  Numbers that are divisible by exactly four different primes.
 * https://oeis.org/A033993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the integers with exactly four distinct prime factors
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with exactly four distinct prime factors (omega(n) = 4) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Static data for memoisation */
    static seq_t *terms = NULL;   /* array of found terms          */
    static size_t  tcnt  = 0;     /* number of terms currently stored */
    static size_t  tcap  = 0;     /* capacity of that array         */
    static idx_t   checked = 0;   /* last integer examined          */

    /* If we do not yet have n terms, continue searching */
    while (tcnt < n) {
        ++checked;                           /* examine next integer */
        if (count_distinct_primes(checked) == 4) {
            /* Ensure we have room in the array */
            if (tcnt == tcap) {
                size_t newcap = tcap ? tcap << 1 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) {               /* allocation failure, unlikely */
                    /* In a real system we would handle this error */
                    return 0;
                }
                terms = tmp;
                tcap  = newcap;
            }
            terms[tcnt++] = (seq_t)checked;
        }
    }
    return terms[n - 1];
}
