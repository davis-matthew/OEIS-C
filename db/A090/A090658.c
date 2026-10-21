/*
 * A090658  Numbers n such that n-1 is a prime of the form 4k+3.
 * https://oeis.org/A090658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3284, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090658: Numbers n whose predecessor is a prime ≡3 (mod 4).
   a(n) = p_n + 1 where p_n is the n‑th prime of the form 4k+3. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1: a(1) is defined, a(0) is not */
        return (seq_t)0;

    /* static cache of found primes ≡3 (mod 4) */
    static unsigned long long *primes = NULL;
    static size_t used = 0, cap = 0;
    static unsigned long long cand = 3;   /* smallest 4k+3 number */

    /* initialise storage once */
    if (cap == 0) {
        cap = 16;
        primes = (unsigned long long *)malloc(cap * sizeof(unsigned long long));
        if (!primes)
            abort();
    }

    /* keep generating primes until we have n of them */
    while (used < n) {
        if (is_prime(cand)) {            /* primality test provided by utils */
            if (used == cap) {           /* grow the array if needed */
                size_t newcap = cap << 1;
                unsigned long long *tmp = (unsigned long long *)realloc(primes,
                                                                    newcap * sizeof(unsigned long long));
                if (!tmp)
                    abort();
                primes = tmp;
                cap = newcap;
            }
            primes[used++] = cand;        /* store the new 4k+3 prime */
        }
        cand += 4;                       /* next candidate ≡3 (mod 4) */
    }

    /* the requested term is the (n‑th) prime of the form 4k+3 plus 1 */
    return (seq_t)(primes[n - 1] + 1);
}
