/*
 * A142615  Primes congruent to 19 mod 55.
 * https://oeis.org/A142615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 159, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142615: primes congruent to 19 mod 55.
 * "Compute the n-th prime p such that p ≡ 19 (mod 55)."
 * The function tests candidates starting at 19 and incrementing by 55
 * until the n-th prime is found. */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n ≥ 1; this guard is only
         * for safety in case of accidental zero argument. */
        return 0;
    }

    seq_t candidate = 19;      /* first number that satisfies p ≡ 19 (mod 55) */
    idx_t count = 0;           /* number of primes found so far */

    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 55;       /* next candidate with same residue mod 55 */
    }
}
