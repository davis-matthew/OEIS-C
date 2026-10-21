/*
 * A204902  Least prime p such that n divides p-q for some odd prime q<p.
 * https://oeis.org/A204902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate odd primes and track residues modulo n.
   The first odd prime p whose residue modulo n matches
   a previously seen odd prime gives the desired value. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/next_prime.h" /* provides uint64_t next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    /* n is a 64‑bit index, n >= 1 by OEIS definition */
    if (n == 0) return 0;                           /* safety, not used */

    size_t N = (size_t)n;                           /* array size */
    bool *seen = (bool *)malloc(N * sizeof(bool));  /* C++ style cast needed */
    if (!seen) return 0;                            /* out of memory guard */

    /* initialise the table */
    size_t i;
    for (i = 0; i < N; ++i) seen[i] = false;

    uint64_t p = 2;                                 /* start from prime 2 */
    while (1) {
        p = next_prime(p);                          /* next prime after p */

        /* only odd primes matter as q must be odd */
        if (p <= 2) continue;                      /* skip 2, though next_prime(2)=3 */

        uint64_t r = p % n;                        /* residue modulo n */
        if (seen[(size_t)r]) {                     /* a previous odd prime had same residue */
            free(seen);
            return (seq_t)p;
        }
        seen[(size_t)r] = true;                    /* record this odd prime's residue */
    }
}
