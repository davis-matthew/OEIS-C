/*
 * A049092  Primes p such that p-1 is not squarefree.
 * https://oeis.org/A049092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049092: primes p such that p-1 is not squarefree (mu(p-1)=0) */
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* Static state for memoization */
    static seq_t *qualPrimes = NULL;        /* array of qualifying primes */
    static size_t qualCap   = 0;            /* capacity of the array */
    static size_t qualCnt   = 0;            /* number of stored primes */
    static uint64_t lastPrime = 2;          /* last prime examined */

    /* Generate more qualifying primes until we have the nth */
    while ((idx_t)qualCnt < n) {
        /* Find next prime */
        lastPrime = next_prime(lastPrime + 1);

        /* Test whether lastPrime-1 is squarefree */
        uint64_t m = lastPrime - 1;
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64(m, pr, ex, &cnt);

        int squarefree = 1;  /* assume squarefree until shown otherwise */
        for (size_t i = 0; i < cnt; ++i) {
            if (ex[i] > 1) {           /* squared factor found */
                squarefree = 0;
                break;
            }
        }

        if (!squarefree) {            /* qualifies */
            /* Resize array if necessary */
            if (qualCnt == qualCap) {
                size_t newCap = qualCap ? qualCap * 2 : 16;
                seq_t *tmp = seq_realloc(qualPrimes, qualCap, newCap);
                qualPrimes = tmp;
                qualCap = newCap;
            }
            qualPrimes[qualCnt++] = (seq_t)lastPrime;
        }
    }

    /* Return the requested element (1-based index) */
    return qualPrimes[n - 1];
}
