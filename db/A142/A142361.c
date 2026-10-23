/*
 * A142361  Primes congruent to 10 mod 47.
 * https://oeis.org/A142361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142361: primes congruent to 10 (mod 47) */
/* a(n) is the n‑th prime p such that p ≡ 10 (mod 47).  The function
   performs a small exhaustive search, testing each candidate of the
   form 47*k+10 for primality. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is undefined */
        return 0;

    idx_t count   = 0;
    seq_t cand    = 10;          /* first number ≡ 10 (mod 47) */
    while (1) {
        if (is_prime((idx_t)cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand += (seq_t)47;
    }
}
