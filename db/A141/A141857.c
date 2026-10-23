/*
 * A141857  Primes congruent to 10 mod 11.
 * https://oeis.org/A141857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 10 (mod 11)
 * Range:     u8 n <= 5, u16 n <= 663, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         Candidate numbers can be written as 22*r + 21 with r = 1,2,3,...
 *         Only those candidates which are prime are stored in a static cache
 *         and returned for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"   /* provides bool isPrimeTrial(seq_t); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is offset 1; undefined for n<1 */

    /* static state to cache found primes and the next candidate index */
    static seq_t *primes = NULL;
    static size_t  cap   = 0;
    static size_t  count = 0;       /* number of primes already stored   */
    static idx_t  r      = 1;       /* current index r for candidates   */

    /* Allocate / grow the primes array lazily */
    if (primes == NULL) {
        cap = 32;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (primes == NULL) return 0;           /* allocation failure */
    }

    /* Extend the cache until it contains at least n primes */
    while (count < n) {
        seq_t cand = (seq_t)22 * (seq_t)r + (seq_t)21;  /* candidate 22*r+21 */
        if (isPrimeTrial(cand)) {
            if (count >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (tmp == NULL) return 0;                /* allocation failure */
                primes = tmp;
            }
            primes[count++] = cand;
        }
        ++r;
    }

    /* Return the n-th prime (array is 0‑based) */
    return primes[n - 1];
}
