/*
 * A007934  Some nontrivial permutation of digits is a prime.
 * https://oeis.org/A007934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- a(n) is the n-th number k > 10 such that some
 *            non-identity rearrangement of k's digits (leading zeros allowed)
 *            is prime; rearranging equal digits counts, so a prime with a
 *            repeated digit is a member
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_sequence, my_is_prime, search_perms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/in_sequence.h"
#include "../utils/my_is_prime.h"
#include "../utils/search_perms.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t next_k = 11;

seq_t a(idx_t n) {
    while (nterms < n) {
        if (in_sequence(next_k)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
                capterms = nc;
            }
            terms[nterms++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
