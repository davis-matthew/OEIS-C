/*
 * A140536  Primes congruent to 13 or 197 mod 210.
 * https://oeis.org/A140536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 240, u32 n <= 267, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 13 or 197 (mod 210) */

/*  a(n) = the nth prime p such that p ≡ 13 or 197 (mod 210) */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t count = 0;          /* number of primes found so far */
    idx_t k = 0;              /* current block of 210 */

    for (;;) {
        /* candidate 1: 210*k + 13 */
        seq_t cand1 = (seq_t)210 * (seq_t)k + 13;
        if (is_prime_big(cand1)) {
            count++;
            if (count == n) return cand1;
        }

        /* candidate 2: 210*k + 197 */
        seq_t cand2 = (seq_t)210 * (seq_t)k + 197;
        if (is_prime_big(cand2)) {
            count++;
            if (count == n) return cand2;
        }

        k++;
    }
}
