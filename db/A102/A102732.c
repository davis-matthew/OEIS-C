/*
 * A102732  Primes of the form 13n+5.
 * https://oeis.org/A102732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p with p % 13 == 5. * This is implemented by
 *            generating primes of that form sequentially * using the
 *            is_prime() helper from utils
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) /* sequence starts at offset 1 */
        return 0;

    /* static state for cached results */
    static seq_t *primes    = NULL;
    static idx_t  count     = 0;     /* number of primes already stored */
    static idx_t  capacity  = 0;     /* allocated size of primes array */
    static uint64_t cand     = 5;    /* first candidate 13*0+5 */

    /* generate primes until we have at least n of them */
    while (count < n) {
        /* find next prime of the form 13k+5 */
        while (!is_prime(cand))
            cand += 13;

        /* grow the storage array if needed */
        if (count == capacity) {
            idx_t new_cap = capacity ? capacity << 1 : 128;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);           /* abort if allocation fails */
            primes   = tmp;
            capacity = new_cap;
        }

        /* store the prime */
        primes[count] = (seq_t)cand;
        ++count;
        cand += 13;          /* next possible candidate */
    }

    return primes[n - 1];
}
