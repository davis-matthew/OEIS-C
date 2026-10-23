/*
 * A171600  Primes of the form floor(T/8) where T is a triangular number.
 * https://oeis.org/A171600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 104, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form floor(T/8) where T is triangular.
   a(n) = the n‑th prime among floor(t*(t+1)/16) for t = 1,2,…
   (t starts at 1, the sequence is indexed from 1). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) -> bool */

seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many primes found so far */
    idx_t t = 1;       /* current t in T = t(t+1)/2 */

    for (;; ++t) {
        /* Compute floor(t*(t+1)/16) safely in idx_t, then cast to seq_t */
        idx_t temp = t * (t + 1) / 16;
        seq_t val  = (seq_t)temp;

        if (val > 1 && is_prime_big(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
