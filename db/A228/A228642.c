/*
 * A228642  Squares of primes mod 100.
 * https://oeis.org/A228642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   a(n) is the nth smallest distinct residue modulo 100 of prime
 *            squares (excluding leading zeros)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of computed residues */
    static bool initialized = false;
    static seq_t residues[100];
    static int residues_len = 0;

    if (!initialized) {
        /* find all residues of p^2 (mod 100) for primes p */
        uint64_t *primes = NULL;
        size_t prime_count = 0;
        const idx_t limit = 20000;   /* sufficiently large to capture all residues */

        build_primes_upto(limit, &primes, &prime_count);

        bool seen[100] = {0};
        residues_len = 0;

        for (size_t idx = 0; idx < prime_count; ++idx) {
            uint64_t p = primes[idx];
            seq_t r = ((seq_t)p * (seq_t)p) % (seq_t)100;
            if (!seen[(int)r]) {
                seen[(int)r] = true;
                residues[residues_len++] = r;
                if (residues_len == 12) /* we know there are only 12 distinct residues */
                    break;
            }
        }
        free(primes);

        /* sort residues in ascending order */
        for (size_t i = 1; i < (size_t)residues_len; ++i) {
            seq_t key = residues[i];
            size_t j = i;
            while (j > 0 && residues[j - 1] > key) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }

        initialized = true;
    }

    if (n == 0 || n > residues_len)
        return 0;         /* undefined for out-of-range indices */
    return residues[(size_t)n - 1];
}
