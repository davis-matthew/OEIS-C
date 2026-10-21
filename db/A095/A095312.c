/*
 * A095312  Primes in whose binary expansion the number of 1-bits is > 6 +
 *          number of 0-bits.
 * https://oeis.org/A095312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, floor_log2_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose binary expansion has more 1‑bits than 6 plus the
 * number of 0‑bits.  Sequence A095312. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */

    /* static state for incremental generation and caching */
    static uint64_t next_candidate = 3;   /* next odd number to test */
    static seq_t *qual = NULL;            /* array of qualifying primes */
    static size_t qual_len = 0;           /* number of elements stored */
    static size_t qual_cap = 0;           /* allocated capacity */

    /* generate qualifying primes until we have at least n of them */
    while (qual_len < (size_t)n) {
        for (uint64_t cand = next_candidate; ; cand += 2) {
            if (!is_prime(cand))
                continue;

            /* count ones and zeros in binary representation */
            idx_t ones = 0;
            idx_t tmp  = cand;
            while (tmp) {
                ones++;
                tmp &= tmp - 1;          /* remove lowest set bit */
            }
            idx_t bitlen = floor_log2_uint64(cand) + 1;
            idx_t zeros  = bitlen - ones;

            if (ones > 6 + zeros) {
                /* store the qualifying prime */
                ensure_capacity(qual_len + 1, &qual_cap,
                                (void **)&qual, sizeof(seq_t));
                qual[qual_len++] = (seq_t)cand;

                /* next search starts after this prime */
                next_candidate = cand + 2;
                break;
            }
        }
    }

    return qual[n - 1];
}
