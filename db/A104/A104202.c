/*
 * A104202  Differences of straddle primes.
 * https://oeis.org/A104202
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = difference between the nearest primes that straddle the
 *            (n‑3)th composite number
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* undefined for n < 4; any value acceptable below offset */
    if (n < 4) return 0;

    /* number of composite needed (1‑based) */
    idx_t k = n - 3;

    /* static storage for discovered composites */
    static uint64_t *comps = NULL;
    static size_t comps_len = 0;
    static size_t comps_cap = 0;
    static uint64_t next_candidate = 4;   /* next number to test for compositeness */

    /* extend list until we have at least k composites */
    while (comps_len < (size_t)k) {
        /* allocate more space if necessary */
        if (comps_len >= comps_cap) {
            size_t new_cap = comps_cap == 0 ? 16 : comps_cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
            if (!tmp) abort();            /* very unlikely */
            comps = tmp;
            comps_cap = new_cap;
        }

        /* find next composite */
        while (is_prime(next_candidate))
            ++next_candidate;
        comps[comps_len++] = next_candidate++;
    }

    uint64_t c = comps[k - 1];          /* the (n-3)th composite (1‑based) */

    /* nearest prime before c */
    uint64_t p = c - 1;
    while (!is_prime(p)) --p;

    /* nearest prime after c */
    uint64_t q = c + 1;
    while (!is_prime(q)) ++q;

    return (seq_t)(q - p);
}
