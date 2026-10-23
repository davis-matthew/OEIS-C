/*
 * A141885  Primes congruent to 13 mod 20.
 * https://oeis.org/A141885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 830, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141885: primes congruent to 13 modulo 20.
   Computed by brute‑force search. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; values below are ignored */
        return 0;

    idx_t count = 0;
    idx_t cand  = 13;          /* first candidate ≡13 (mod 20) */

    while (1) {
        if (is_prime(cand)) {  /* is_prime: bool is_prime(idx_t n) */
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 20;           /* next number with residue 13 mod 20 */
    }
}
