/*
 * A214752  Primes in the sequence of tribonacci numbers starting 1, 1, 0
 *          (A081172).
 * https://oeis.org/A214752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime among the Tribonacci numbers with initial
 *            terms T(1)=1, T(2)=1, T(3)=0 and recurrence
 *            T(k)=T(k-1)+T(k-2)+T(k-3). (The sequence of such primes is
 *            A214752 in OEIS.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static storage for the list of primes found so far and
       the current Tribonacci window.  All variables are initialised
       once for the lifetime of the program. */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t t1 = 1, t2 = 1, t3 = 0;

    /* Generate new Tribonacci numbers until we have at least n primes. */
    while (primes_cnt < (size_t)n)
    {
        /* next T(k) = T(k-1)+T(k-2)+T(k-3) */
        seq_t next = t1 + t2 + t3;

        /* shift window for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = next;

        /* If the new Tribonacci number is prime, store it. */
        if (is_prime_big(t3))
        {
            if (primes_cnt == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                void *tmp = realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* out of memory */
                primes = (seq_t*)tmp; /* cast for C and C++ */
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = t3;
        }
    }

    /* Return the requested prime (n is 1‑based). */
    return primes[n - 1];
}
