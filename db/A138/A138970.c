/*
 * A138970  Positions of the primes congruent to 1 mod 4 when all primes
 *          except 2 are listed in order.
 * https://oeis.org/A138970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over the odd integers >2, test primality,
   count the primes that are ≡ 1 (mod 4), and return the position
   (index in the list of primes excluding 2) of the n‑th such prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0)                /* offset is 1; behaviour for n==0 is undefined */
        return 0;

    idx_t count = 0;           /* how many primes ≡ 1 mod 4 have been seen */
    idx_t pos   = 0;           /* position of the current prime in the list of primes >2 */
    idx_t cand  = 3;           /* current integer being tested (starts with prime 3) */

    while (true) {
        if (is_prime(cand)) {               /* cand is prime */
            pos++;                           /* advance position counter */
            if ((cand & 3) == 1) {           /* cand ≡ 1 (mod 4)  */
                ++count;
                if (count == n)              /* the n‑th such prime found */
                    return (seq_t)pos;       /* return its position */
            }
        }
        cand += 2;                           /* test next odd number */
    }
}
