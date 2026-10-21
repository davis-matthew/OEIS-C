/*
 * A080567  1 + Sum_{k=2..n} 2^((prime(k)-1)/2).
 * https://oeis.org/A080567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=2..n} 2^((prime(k)-1)/2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cached values of the sequence.  cum[i] holds a(i+1). */
    static seq_t *cum   = NULL;
    static size_t cum_len = 0;
    static size_t cum_cap = 0;
    /* Candidate for the next prime to use.  Start at 3 because
       the first prime 2 is not part of the sum. */
    static uint64_t next_prime = 3;

    /* Expand the cache until we have computed a(n). */
    while (cum_len < n) {
        /* Initial allocation.  a(1)=1. */
        if (cum_len == 0) {
            cum_cap = 16;
            cum     = (seq_t *)malloc(cum_cap * sizeof(seq_t));
            if (!cum) return 0;           /* allocation failed */
            cum[0] = (seq_t)1;            /* a(1) = 1 */
            cum_len = 1;
            continue;
        }

        /* Find the next prime number. */
        while (!is_prime(next_prime))
            ++next_prime;

        /* Compute the contribution for this prime. */
        idx_t exp = (idx_t)((next_prime - 1) / 2);
        seq_t term = pow2(exp);

        /* Grow the array if necessary. */
        if (cum_len == cum_cap) {
            size_t new_cap = cum_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed */
            cum = tmp;
            cum_cap = new_cap;
        }

        /* Store the new cumulative value. */
        cum[cum_len] = cum[cum_len - 1] + term;
        ++cum_len;

        /* Advance to the next candidate for the following prime. */
        ++next_prime;
    }

    return cum[n - 1];
}
