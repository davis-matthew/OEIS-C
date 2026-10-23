/*
 * A142106  Primes congruent to 19 mod 36.
 * https://oeis.org/A142106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 538, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142106: primes congruent to 19 modulo 36.
   a(n) is the n‑th prime p with p ≡ 19 (mod 36). */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n < 1 is irrelevant */

    idx_t count = 0;
    idx_t cand  = 19;              /* first member of the residue class 19 (mod 36) */

    while (true) {
        if (is_prime(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        cand += 36;                 /* advance to the next number ≡ 19 (mod 36) */
    }
}
