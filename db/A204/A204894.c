/*
 * A204894  Least prime p such that n divides p-q for some prime q<p.
 * https://oeis.org/A204894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that ∃ prime q < p with n | (p – q)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: scan primes in increasing order, keeping a table of residues
 * modulo n that have already appeared among earlier primes.
 * The first time we see a residue that was seen before,
 * the current prime is the desired least prime p. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* Returns the sequence term a(n) of OEIS A204894. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* outside the offset */

    /* Allocate a boolean array of size n (the residue classes modulo n). */
    size_t N = (size_t)n;
    bool *seen = (bool *)calloc(N, sizeof(bool));
    if (!seen) return 0;                          /* allocation failure */

    /* Start with the first prime. */
    uint64_t p = 2;
    while (1) {
        /* Compute the residue of the current prime modulo n. */
        seq_t r = (seq_t)p % (seq_t)n;
        size_t idx = (size_t)r;

        /* If this residue was seen before, we found the least prime p. */
        if (seen[idx]) {
            seq_t ans = (seq_t)p;
            free(seen);
            return ans;
        }

        /* Mark this residue as seen and advance to the next prime. */
        seen[idx] = true;
        p = next_prime(p);                        /* next prime > previous p */
    }

    /* This point is never reached. */
    free(seen);
    return 0;
}
