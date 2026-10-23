/*
 * A038938  Primes p such that 58 is not a square mod p.
 * https://oeis.org/A038938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p for which 58 is a quadratic non‑residue
 *            modulo p (i.e. Jacobi symbol (58|p) = -1). The primes 2 and 29,
 *            which divide 58, are excluded. The sequence is generated
 *            incrementally
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/jacobi_symbol.h"

/* Storage for the sequence.  200 000 slots comfortably hold >10 000 terms. */
static seq_t static_vals[200000];
static idx_t  count = 0;           /* number of terms already stored */
static uint64_t last_checked = 1; /* last integer examined (prime) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    /* Extend the cache until it contains at least n terms. */
    while (count < n) {
        /* Find the next prime greater than `last_checked`. */
        uint64_t cand = last_checked + 1;
        /* Ensure we start with a prime candidate (2 or odd). */
        if (cand <= 2) cand = 2;
        else if ((cand & 1ULL) == 0) cand++;          /* make it odd */

        /* Search for a prime. */
        for (;;) {
            if (is_prime_small(cand)) {
                last_checked = cand;                 /* remember last prime */

                /* Skip the primes that divide 58. */
                if (cand != 2 && cand != 29) {
                    /* Jacobi symbol (58|cand) should be –1. */
                    if (jacobi_symbol(58, cand) == -1) {
                        static_vals[count++] = (seq_t)cand;
                    }
                }
                break;                               /* next candidate will be > cand */
            }
            cand += 2;                               /* skip even numbers */
        }
    }
    return static_vals[n - 1];
}
