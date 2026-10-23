/*
 * A142445  Primes congruent to 37 mod 49.
 * https://oeis.org/A142445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime that is congruent to 37 mod 49
   (OEIS A142445).  It uses a dynamic sieve: it keeps a cache of all
   primes up to a current limit and enlarges that limit (doubling each
   time) until the cache contains at least n primes in the required
   residue class.  No floating‑point arithmetic or hard‑coded tables
   are used. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 0 is not a valid index – return 0 for safety */
    if (n == 0)
        return 0;

    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t cur_limit = 0;

    for (;;) {
        /* initialise the sieve if this is the first call */
        if (cur_limit == 0) {
            cur_limit = 100;                      /* start with a small limit */
            build_primes_upto(cur_limit, &primes, &primes_cnt);
        }

        /* count primes congruent to 37 (mod 49) */
        size_t found = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            if ((primes[i] % 49ULL) == 37ULL) {
                ++found;
                if (found == (size_t)n)
                    return (seq_t)primes[i];
            }
        }

        /* not enough primes yet: enlarge the sieve and rebuild */
        free(primes);
        cur_limit = cur_limit * 2 + 100;          /* simple growth factor */
        build_primes_upto(cur_limit, &primes, &primes_cnt);
    }
}
