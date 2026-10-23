/*
 * A142679  Primes congruent to 23 mod 57.
 * https://oeis.org/A142679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 194, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 23 modulo 57 (OEIS A142679). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/*  The sequence  a(n)  is defined for  n ≥ 1.
    a(1) is the first prime ≡ 23 (mod 57).  */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;   /* offset starts at 1 */

    /* Candidate primes start at 23 and are all odd. */
    uint64_t cand = 23u;
    idx_t    found = 0u;

    while (1) {
        if (is_prime(cand)) {
            if ((cand % 57u) == 23u) {
                ++found;
                if (found == n) return (seq_t)cand;
            }
        }
        cand += 2u;        /* skip even numbers – all members are odd */
    }
}
