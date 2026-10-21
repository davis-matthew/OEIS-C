/*
 * A069754  Counts transitions between prime and nonprime to reach the number
 *          n.
 * https://oeis.org/A069754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 727, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069754:  a(n) = number of transitions between prime and non‑prime
 *  while stepping from 1 to n.
 *
 *  Implementation:  incremental Sieve of Eratosthenes + prefix sum of
 *  transitions.  The function is cached so repeated calls with any order
 *  of indices are handled efficiently.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool _is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /*  static cache  */
    static bool  *prime   = NULL;   /* prime[i] : 1 if i is prime, 0 otherwise */
    static seq_t *pref    = NULL;   /* pref[i]  : a(i) */
    static size_t cached  = 0;       /* highest index currently cached    */

    if (n == 0)           /*  offset starts at 1, but provide safe return  */
        return 0;

    if (n > cached) {
        size_t new_n = (size_t)n + 1;          /* allocate up to n inclusive */

        /*  Re‑allocate prime array  */
        bool  *prime_new = (bool *)realloc(prime, new_n);
        if (!prime_new)
            return 0;  /* out of memory, silently return 0 */
        prime   = prime_new;

        /*  Initialise all as potential primes  */
        for (size_t i = 2; i < new_n; ++i)
            prime[i] = true;
        if (new_n > 0) prime[0] = false;
        if (new_n > 1) prime[1] = false;

        /*  Apply Sieve of Eratosthenes  */
        for (size_t p = 2; p * p < new_n; ++p) {
            if (prime[p]) {
                for (size_t m = p * p; m < new_n; m += p)
                    prime[m] = false;
            }
        }

        /*  Re‑allocate prefix array  */
        seq_t *pref_new = (seq_t *)realloc(pref, new_n * sizeof(seq_t));
        if (!pref_new)
            return 0;
        pref = pref_new;

        /*  Compute transition counts up to new_n-1  */
        pref[0] = 0;                       /* a(0) unused, set to 0 */
        pref[1] = 0;                       /* a(1) = 0 */

        for (size_t i = 2; i < new_n; ++i) {
            bool prev = prime[i - 1];
            bool cur  = prime[i];
            pref[i] = (seq_t)(pref[i - 1] + (prev != cur));
        }

        cached = (size_t)n;
    }

    return pref[n];
}
