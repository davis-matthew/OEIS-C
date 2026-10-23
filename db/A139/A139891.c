/*
 * A139891  Primes of the form 7x^2+30y^2.
 * https://oeis.org/A139891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A139891: Primes of the form 7x^2 + 30y^2 (OEIS) */
/* a(n) is the n‑th prime p such that p ≡ {7,37,127,247,253,277,373,463,487,
   583,613,757,823} (mod 840). The function searches for the n‑th such prime
   by testing integers sequentially with the helper is_prime(idx_t). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

static const unsigned short RESIDUES[] = {
    7, 37, 127, 247, 253, 277, 373,
    463, 487, 583, 613, 757, 823
};
static const unsigned short NUM_RESIDUES = sizeof(RESIDUES)/sizeof(RESIDUES[0]);

static inline bool is_desired_residue(idx_t p)
{
    unsigned short r = (unsigned short)(p % 840ULL);
    for (unsigned short i = 0; i < NUM_RESIDUES; ++i)
        if (r == RESIDUES[i])
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 2;

    while (1) {
        /* Skip even numbers after 2 to reduce work */
        if (candidate > 2 && (candidate & 1ULL) == 0) {
            ++candidate;
            continue;
        }

        if (is_prime(candidate) && is_desired_residue(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }

        ++candidate;
    }
}
