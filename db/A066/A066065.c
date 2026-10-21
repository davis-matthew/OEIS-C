/*
 * A066065  a(n) = smallest prime q such that in decimal notation the
 *          concatenation prime(n)q yields a prime ( = A066064(n)).
 * https://oeis.org/A066065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = min{q : q is prime and concat(prime(n), q) is prime}
 * Range:     u8 n <= 10, u16 n <= 1240, u32 n <= 89, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   build_primes_upto, concat_parts, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 7, a(3) = 3, a(4) = 3, ... as in OEIS A066065 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑indexed).  get_prime returns a 32‑bit value. */
    uint32_t p = get_prime(n);

    /* Lazily build a list of candidate primes q. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (prime_list == NULL) {
        /* 2 000 000 is more than enough for a(n) < prime(n) for n≤10 000. */
        const idx_t LIMIT = 2000000;
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }

    /* Search for the smallest q such that concat(p,q) is prime. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        seq_t candidate = concat_parts((idx_t)p, (idx_t)q);
        if (is_prime_big(candidate)) {
            return (seq_t)q;
        }
    }

    /* In practice the previous loop always finds an answer.  If it does not,
       extend the prime table and continue searching (unlikely for n≤10 000). */
    while (1) {
        /* double the limit each time */
        const idx_t new_limit = prime_count ? prime_count * 2 : 2000000;
        build_primes_upto(new_limit, &prime_list, &prime_count);

        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_list[i];
            seq_t candidate = concat_parts((idx_t)p, (idx_t)q);
            if (is_prime_big(candidate)) {
                return (seq_t)q;
            }
        }
    }
}
