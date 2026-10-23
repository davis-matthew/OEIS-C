/*
 * A139651  Primes of the form x^2 + 210*y^2.
 * https://oeis.org/A139651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 384, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 + 210*y^2
   Computed by brute‑force search using the residue classes
   specified in the OEIS entry.  No hard‑coded values are used. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Return the n‑th prime of the form x^2 + 210*y^2,  n >= 1.  */
seq_t a(idx_t n)
{
    /* 12 residue classes mod 840 for such primes */
    static const int residues[12] = {
        1, 121, 169, 211, 289,
        331, 361, 379, 499, 529,
        571, 739
    };
    static bool residue_map[840];
    static bool residue_initialized = false;

    /* Initialise residue lookup table on first call */
    if (!residue_initialized) {
        memset(residue_map, 0, sizeof(residue_map));
        for (int i = 0; i < 12; ++i) {
            residue_map[residues[i]] = true;
        }
        residue_initialized = true;
    }

    /* Static cache of primes already found (up to a fixed limit) */
    /* This limit is safe for n up to about 10 000 as required. */
    static seq_t primes[20000];
    static uint64_t primes_len = 0;
    static uint64_t last_checked = 1;  /* last tested integer (exclusive) */

    /* Expand cache until it contains at least n primes */
    while (primes_len < n) {
        ++last_checked;
        int rem = (int)(last_checked % 840);
        if (!residue_map[rem]) continue;   /* not in a valid residue class */

        seq_t val = (seq_t)last_checked;
        if (is_prime_big(val)) {
            primes[primes_len++] = val;
        }
    }

    return primes[n - 1];
}
