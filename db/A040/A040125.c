/*
 * A040125  Primes p such that x^4 = 12 has no solution mod p.
 * https://oeis.org/A040125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that the congruence x⁴ = 12 (mod p)
 *            has no solution (OEIS A040125)
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *found = NULL;
    static size_t count  = 0;
    static size_t cap    = 0;
    static uint64_t next_candidate = 5; /* start with the first odd > 3 */

    while (count < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;           /* skip even numbers */

        if (is_prime_small(cand)) {
            if (cand % 4 == 3) {
                /* p ≡ 3 (mod 4): 12 is a fourth‑power residue iff
                   12^{(p‑1)/2} ≡ 1 (Legendre symbol +1).  We want -1. */
                seq_t res = mod_pow((seq_t)12, (cand - 1) / 2, (seq_t)cand);
                if (res == (seq_t)(cand - 1)) { /* Legendre symbol –1 */
                    if (count == cap) {
                        cap = cap ? cap * 2 : 16;
                        found = (seq_t *)realloc(found, cap * sizeof(seq_t));
                    }
                    found[count++] = (seq_t)cand;
                }
            } else { /* cand % 4 == 1 */
                /* p ≡ 1 (mod 4): 12 is a fourth‑power residue iff
                   12^{(p‑1)/4} ≡ 1.  We want it not equal to 1. */
                seq_t res = mod_pow((seq_t)12, (cand - 1) / 4, (seq_t)cand);
                if (res != (seq_t)1) {
                    if (count == cap) {
                        cap = cap ? cap * 2 : 16;
                        found = (seq_t *)realloc(found, cap * sizeof(seq_t));
                    }
                    found[count++] = (seq_t)cand;
                }
            }
        }
    }
    return found[(size_t)(n - 1)];
}
