/*
 * A142426  Primes congruent to 15 mod 49.
 * https://oeis.org/A142426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime ≡ 15 (mod 49)
 * Range:     u8 n <= 2, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142426: Primes congruent to 15 mod 49.  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for indices below 1, but safe to return 0 */

    uint64_t candidate = 15;           /* first number ≡ 15 (mod 49) */
    idx_t  count       = 0;            /* number of primes found so far */

    while (1) {
        /* Only check numbers congruent to 15 mod 49 */
        if (is_prime((idx_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 49;               /* next number with residue 15 mod 49 */
    }
}
