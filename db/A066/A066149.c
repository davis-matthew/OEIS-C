/*
 * A066149  Primes with an odd number of 0's in binary expansion.
 * https://oeis.org/A066149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3594, u32 n <= 9297, u64 n <= 9297,
 *            u128 n <= 9297, big any n
 * Helpers:   floor_log2_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime whose binary representation contains an odd
   number of zeros.  The function generates primes in increasing order
   until the requested count is reached; results are cached so that
   future calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes. */
    static seq_t *qual = NULL;      /* array of primes satisfying the condition */
    static size_t qual_cap = 0;     /* current capacity of array */
    static size_t qual_cnt = 0;     /* number of stored primes */
    static uint64_t next_cand = 2;  /* next integer to test */

    /* Keep generating primes until we have at least n of them. */
    while (qual_cnt < n) {
        if (is_prime(next_cand)) {
            /* Count zeros in binary representation. */
            uint64_t bits  = floor_log2_uint64(next_cand) + 1;
            uint64_t ones  = __builtin_popcountll(next_cand);
            uint64_t zeros = bits - ones;
            if ((zeros & 1UL) != 0) {            /* odd number of zeros */
                /* Resize cache if necessary. */
                if (qual_cnt == qual_cap) {
                    qual_cap = qual_cap ? qual_cap * 2 : 256;
                    seq_t *tmp = (seq_t*)realloc(qual, qual_cap * sizeof(seq_t));
                    /* In the unlikely event of allocation failure,
                       just return zero (the caller will reject it) */
                    if (!tmp) {
                        return 0;
                    }
                    qual = tmp;
                }
                qual[qual_cnt++] = (seq_t)next_cand;
            }
        }
        next_cand++;
    }

    return qual[n - 1];
}
