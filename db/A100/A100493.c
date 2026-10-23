/*
 * A100493  a(n) = n + n-th semiprime.
 * https://oeis.org/A100493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + A001358(n) (nth semiprime)
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_semiprime_uint64.h"

seq_t a(idx_t n) {
    /* static storage for the sequence of semiprimes */
    static seq_t *semiprimes = NULL;
    static size_t semiprimes_cap = 0;
    static idx_t semiprimes_len = 0;
    static idx_t next_candidate = 4; /* next integer to test for being a semiprime */

    /* Ensure we have at least n semiprimes computed */
    while (semiprimes_len < n) {
        /* Expand the storage if necessary */
        if (semiprimes_len >= semiprimes_cap) {
            size_t new_cap = semiprimes_cap ? semiprimes_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(semiprimes, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed */
            semiprimes = tmp;
            semiprimes_cap = new_cap;
        }

        /* Test the current candidate */
        if (is_semiprime_uint64((uint64_t)next_candidate)) {
            semiprimes[semiprimes_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return ((seq_t)n) + semiprimes[n - 1];
}
