/*
 * A142214  Primes congruent to 17 mod 41.
 * https://oeis.org/A142214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 163, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142214: primes congruent to 17 modulo 41.
 * The function searches candidates of the form 41*k+17,
 * tests each with is_prime_big, and returns the n-th found prime.
 * This works for any seq_t (fixed‑size or arbitrary precision) and any idx_t.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence offset starts at 1 */

    idx_t  count = 0;
    idx_t  cand  = 17;                     /* first candidate ≡ 17 mod 41 */

    while (1) {
        seq_t val = (seq_t)cand;
        if (is_prime_big(val)) {
            ++count;
            if (count == n)
                return val;
        }
        cand += 41;                        /* next number ≡ 17 (mod 41) */
    }
}
