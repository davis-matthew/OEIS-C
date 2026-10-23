/*
 * A101219  Primes that are a concatenation of 3, 5 and a prime.
 * https://oeis.org/A101219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th prime that is the concatenation of 3, 5 and a *
 *            prime p, i.e., numbers of the form 35 * 10^len(p) + p that are
 *            prime
 * Range:     u8 none, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_seq, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple incremental search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_seq.h"
#include "../utils/num_digits.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* value for indices < 1 is irrelevant */

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_used = 0;
    static idx_t cache_cap  = 0;
    static uint64_t next_q  = 2;          /* next integer to test for primality */

    while (cache_used < n) {
        /* Find next prime q */
        while (!is_prime_seq((seq_t)next_q)) ++next_q;
        uint64_t q = next_q;
        ++next_q;          /* advance for next search */

        /* compute 35 * 10^len(q) + q */
        int d = num_digits(q);
        bool cand_is_prime = false;
        seq_t candidate;

        if (sizeof(seq_t) <= 8) { /* seq_t fits in 64 bits */
            uint64_t pow10 = 1;
            bool overflow = false;
            for (int i = 0; i < d; ++i) {
                if (pow10 > UINT64_MAX / 10) { overflow = true; break; }
                pow10 *= 10;
            }
            if (!overflow) {
                if (pow10 > 0 && pow10 > UINT64_MAX / 35) {
                    overflow = true;
                }
            }
            if (!overflow) {
                uint64_t cand64 = 35 * pow10 + q;
                if (cand64 >= 3 && is_prime_seq((seq_t)cand64)) {
                    cand_is_prime = true;
                    candidate = (seq_t)cand64;
                }
            }
        } else { /* seq_t has arbitrary precision */
            unsigned __int128 pow10 = 1;
            for (int i = 0; i < d; ++i) pow10 *= (unsigned __int128)10;
            unsigned __int128 cand128 = (unsigned __int128)35 * pow10 + (unsigned __int128)q;
            candidate = (seq_t)cand128;
            cand_is_prime = is_prime_seq(candidate);
        }

        if (cand_is_prime) {
            if (cache_used == cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; } /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_used++] = candidate;
        }
    }

    return cache[n - 1];
}
