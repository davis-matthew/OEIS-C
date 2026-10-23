/*
 * A140039  Primes of the form 28x^2+28xy+73y^2.
 * https://oeis.org/A140039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes represented by 28x^2 + 28xy + 73y^2. The primes are
 *            exactly those congruent to the residues
 *            {73,145,241,409,481,601,745,769,937,985,1249,1273,1657,1777,1825}
 *            (mod 1848). This routine generates these primes in ascending
 *            order using that residue representation and a primality test
 * Range:     u8 n <= 2, u16 n <= 210, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of already found primes */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;

    /* Generation state */
    static uint64_t current_k = 0;     /* polynomial count */
    static size_t residue_idx = 0;     /* next residue to test */

    static const uint16_t residues[15] = {
        73, 145, 241, 409, 481, 601,
        745, 769, 937, 985, 1249, 1273,
        1657, 1777, 1825
    };

    if (n == 0) return 0;         /* undefined for offset 0 – harmless */

    while (cached < (size_t)n) {
        /* Compute the candidate prime for this (k, residue) pair */
        uint64_t pk = 1848ULL * current_k + residues[residue_idx];
        seq_t cand = (seq_t)pk;

        /* Advance to the next residue for the following iteration */
        ++residue_idx;
        if (residue_idx == 15) {   /* finished current k, move to next k */
            residue_idx = 0;
            ++current_k;
        }

        /* Test for primality and store if prime */
        if (cand >= 2 && is_prime_big(cand)) {
            if (cached >= cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* out of memory – unlikely */
                cache = tmp;
                cap = new_cap;
            }
            cache[cached++] = cand;
            if (cached >= (size_t)n) break;
        }
    }
    return cache[n - 1];
}
