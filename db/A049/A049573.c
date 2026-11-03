/*
 * A049573  Primes p such that x^41 = 2 has a solution mod p.
 * https://oeis.org/A049573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p (with offset 1) such that the congruence
 *            x^41 ≡ 2 (mod p) has a solution. For odd primes p the condition
 *            reduces to 41 ∤ (p-1) or 2^((p-1)/41) ≡ 1 (mod p). The special
 *            case p = 2 is always included
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 992 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                       /* safety – offset is 1 */

    /* 2 is always a solution (0^41 ≡ 2 (mod 2)). */
    if (n == 1)
        return (seq_t)2;

    idx_t found = 1;                    /* we already counted prime 2 */
    uint64_t p = 3;                     /* start checking from 3 */

    for (;;) {
        if (is_prime((idx_t)p)) {
            /* Check the condition for odd primes. */
            if (((p - 1) % 41) != 0 ||
                powmod((seq_t)2, (p - 1) / 41, (seq_t)p) == (seq_t)1) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }
        ++p;
    }
}
