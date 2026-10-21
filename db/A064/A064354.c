/*
 * A064354  Distinct values arising in the sequence of the least common
 *          multiples of the first n composite numbers.
 * https://oeis.org/A064354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   _smallest_prime_factor, ensure_capacity, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distinct values arising in the sequence of the least common multiples
   of the first n composite numbers (OEIS A064354). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ensure_capacity.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Static state shared between calls */
    static seq_t *cache = NULL;      /* cache[n-1] = a(n) */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint64_t cur_num = 2;      /* last integer examined */
    static seq_t cur_lcm = (seq_t)1;  /* current running LCM */
    static bool initialized = false;

    /* One-time initialization */
    if (!initialized) {
        /* Allocate space for the first element (a(1) = 1) */
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate composite numbers until we have at least n distinct LCMs */
    while (cache_len < n) {
        /* Find next composite: skip primes using _smallest_prime_factor */
        do {
            cur_num++;
        } while (_smallest_prime_factor(cur_num) == cur_num);

        /* Update running LCM */
        seq_t comp_seq = (seq_t)cur_num;
        seq_t g        = gcd(cur_lcm, comp_seq);
        seq_t new_lcm  = (cur_lcm / g) * comp_seq;

        if (new_lcm != cur_lcm) {
            /* New distinct value – store it */
            ++cache_len;
            ensure_capacity(cache_len, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len - 1] = new_lcm;
            cur_lcm = new_lcm;
        }
        /* otherwise value unchanged – nothing to store */
    }

    return cache[n - 1];
}
