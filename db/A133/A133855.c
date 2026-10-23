/*
 * A133855  Primes p such that p-1 and p+1 are abundant numbers.
 * https://oeis.org/A133855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A133855: primes p such that p‑1 and p+1 are abundant numbers.
 * The function generates primes incrementally and tests the abundance
 * condition using sigma_of; it memoises found primes so it works for
 * arbitrary order of calls.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* gives idx_t next_prime(idx_t) */
#include "../utils/sigma_of.h"    /* gives seq_t sigma_of(idx_t) */

seq_t a(idx_t n)
{
    /* static memoised state */
    static idx_t *qual = NULL;        /* primes satisfying the property   */
    static size_t qcnt = 0;           /* number stored                      */
    static size_t qcap = 0;           /* allocated capacity                 */
    static idx_t last = 1;            /* last value examined for primeness  */

    /* keep generating until we have n qualifying primes */
    while (qcnt < (size_t)n) {
        /* get next prime */
        last = next_prime(last + 1);
        idx_t p = last;

        /* test p-1 and p+1 for abundance */
        seq_t sigma_minus = sigma_of(p - 1);
        seq_t sigma_plus  = sigma_of(p + 1);

        seq_t two_p_minus_1 = (seq_t)(p - 1) * 2;
        seq_t two_p_plus_1  = (seq_t)(p + 1) * 2;

        if (sigma_minus > two_p_minus_1 && sigma_plus > two_p_plus_1) {
            /* store the prime */
            if (qcnt == qcap) {
                size_t new_cap = qcap == 0 ? 16 : qcap * 2;
                idx_t *tmp = (idx_t *)realloc(qual, new_cap * sizeof(idx_t));
                if (!tmp) {            /* allocation failure – return zero */
                    return 0;
                }
                qual = tmp;
                qcap = new_cap;
            }
            qual[qcnt++] = p;
        }
    }

    /* return the nth qualifying prime (1‑based index) */
    return (seq_t)qual[n - 1];
}
