/*
 * A212528  The periodic part of the decimal expansion of prime(n-1) /
 *          prime(n).
 * https://oeis.org/A212528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal periodic part of prime(n-1) / prime(n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 2, a(n) = 0 when prime(n) = 5 (decimal terminates), otherwise
   it is the integer represented by the repeating cycle.  The function
   computes the period by standard long‑division bookkeeping.  No
   hard‑coded values are used. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* defined only for n ≥ 2 */

    /* obtain the two consecutive primes */
    uint32_t p_prev = get_prime(n - 1);    /* prime(n‑1) */
    uint32_t p_curr = get_prime(n);       /* prime(n)   */

    /* terminate if the denominator is 5 (the only prime that gives a
       terminating decimal in this setting). */
    if (p_curr == 5)
        return 0;

    /* long division to generate the recurring digits.
       remainders 1 … p_curr‑1 are possible. */
    static int *seen = NULL;
    static size_t cap = 0;
    size_t need = (size_t)p_curr;

    if (need > cap) {
        free(seen);
        seen = (int *)malloc(need * sizeof(int));
        if (!seen)
            abort();                     /* allocation failure */
        cap = need;
    }
    /* initialise the remainder table with -1 (all bits set). */
    memset(seen, 0xFF, need * sizeof(int));

    seq_t rem   = (seq_t)p_prev;   /* initial remainder = numerator */
    seq_t den   = (seq_t)p_curr;   /* denominator           */
    seq_t period = 0;              /* accumulated digits    */

    for (;;) {
        if (rem == 0)            /* terminating decimal */
            return 0;

        if (seen[(size_t)rem] != -1)  /* remainder repeats – cycle closed */
            return period;

        /* mark this remainder as seen */
        seen[(size_t)rem] = 0;

        /* next digit */
        rem = rem * 10;
        seq_t digit = rem / den;
        rem = rem % den;

        period = period * 10 + digit;
    }
}
