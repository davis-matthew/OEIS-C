/*
 * A097822  Numbers n such that n^2+n+41 (Euler's "prime generating
 *          polynomial") has more than 2 prime factors.
 * https://oeis.org/A097822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that m^2+m+41 has more than two prime
 *            factors (counted with multiplicity). This implementation
 *            increments m, factors m^2+m+41 using factor_u64, and collects
 *            the ones with total prime-exponent sum >2
 * Range:     u8 none, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;
    static idx_t last_checked = 0;          /* last m examined */

    while (len < n) {
        ++last_checked;                     /* try next m */

        unsigned __int128 mm = (unsigned __int128)last_checked;
        unsigned __int128 pp = mm * mm + mm + 41;   /* compute m^2+m+41 safely */
        uint64_t p = (uint64_t)pp;                /* fits into 64 bits here */

        uint64_t primes[64], exps[64];
        size_t cnt;
        factor_u64(p, primes, exps, &cnt);          /* factor p */

        int omega = 0;
        for (size_t i = 0; i < cnt; ++i)
            omega += (int)exps[i];                 /* total multiplicity count */

        if (omega > 2) {                            /* record qualifying m */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_arr) abort();            /* allocation failure */
                terms = new_arr;
                cap = new_cap;
            }
            terms[len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];                      /* a(1) is at index 0 */
}
