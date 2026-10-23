/*
 * A110024  Smallest primes starting a complete three iterations Cunningham
 *          chain of the second kind.
 * https://oeis.org/A110024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for primes p such that
 * p, 2p−1, 4p−3, 8p−7 are prime,
 * 16p−15 is composite,
 * and (p+1)/2 is composite (ensuring p is the first term of the chain). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_size = 0;

    static uint64_t cand = 3;                     /* next odd candidate */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        for (;;) {
            /* Must be prime. */
            if (is_prime(cand)) {
                /* Ensure we are not a continuation of a longer chain. */
                if (is_prime((cand + 1) / 2)) {
                    cand += 2;
                    continue;
                }

                uint64_t q1 = 2 * cand - 1;    /* 2p-1   */
                if (!is_prime(q1)) {
                    cand += 2;
                    continue;
                }

                uint64_t q2 = 4 * cand - 3;    /* 4p-3   */
                if (!is_prime(q2)) {
                    cand += 2;
                    continue;
                }

                uint64_t q3 = 8 * cand - 7;    /* 8p-7   */
                if (!is_prime(q3)) {
                    cand += 2;
                    continue;
                }

                uint64_t q4 = 16 * cand - 15;  /* 16p-15 */
                if (is_prime(q4)) {
                    cand += 2;
                    continue;                 /* would extend chain to length 5 */
                }

                /* All conditions satisfied – add to cache. */
                push_seq(&cache, &cache_cap, &cache_size, (seq_t)cand);
                cand += 2;   /* Move to next candidate. */
                break;
            }
            cand += 2;                           /* skip even numbers */
        }
    }

    return cache[n - 1];
}
