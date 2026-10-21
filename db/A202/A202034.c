/*
 * A202034  Number of distinct prime residues of k^n (mod n), k=0..n-1.
 * https://oeis.org/A202034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime residues of k^n (mod n) for
 *            k=0..n-1 (computed by brute‑force enumeration of all residues)
 * Range:     u8 n <= 429, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* utility functions for modular exponentiation and primality testing */
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* offset: a(1)=0, a(2)=0 are handled by the following condition. */
    if (n < 3ULL) return 0;

    /* Allocate array to mark which residues have already appeared. */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;                /* out of memory → return 0 as fallback */

    seq_t prime_count = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* r = k^n mod n (mod_pow works for all seq_t sizes). */
        seq_t r_seq = mod_pow((seq_t)k, n, (seq_t)n);
        idx_t r = (idx_t)r_seq;         /* r < n, safe cast to idx_t */

        if (!seen[r]) {
            seen[r] = true;
            if (is_prime(r)) ++prime_count;
        }
    }

    free(seen);
    return prime_count;
}
