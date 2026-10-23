/*
 * A175785  Numbers n such that the number of distinct prime divisors of n
 *          does not divide phi(n).
 * https://oeis.org/A175785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 6835, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, count_distinct_primes, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175785: all integers n such that ω(n) does not divide φ(n).
   The function a(n) returns the nth element of this sequence (offset 1). */

#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/calc_phi.h"
#include "../utils/count_distinct_primes.h"

/* External linkage as requested */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;         /* stored sequence terms */
    static size_t cache_size = 0;       /* number of terms stored */
    static size_t cache_cap = 0;        /* capacity of cache array */
    static idx_t last_checked = 1;      /* last integer examined */

    while (cache_size < n) {
        last_checked++;
        /* Compute the number of distinct prime factors (ω) */
        int omega = count_distinct_primes((uint64_t)last_checked);
        if (omega == 0) /* skip n = 1 */
            continue;
        /* Compute φ(n) */
        seq_t phi_val = calc_phi((uint64_t)last_checked);

        /* If ω does not divide φ, store the number */
        if ((phi_val % omega) != 0) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
