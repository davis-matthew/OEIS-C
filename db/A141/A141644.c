/*
 * A141644  Primes of the form (p(2n)-p(n))/(7*2), where p(n)=n-th prime.
 * https://oeis.org/A141644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A141644 (Primes of the form (p(2k)-p(k))/(7*2)),
   a(n) = the nth such prime, with a(1) = 3. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cached prime list and its count. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure we have at least `need` primes in prime_list. */
static void ensure_primes(size_t need) {
    if (prime_count >= need)
        return;

    if (prime_limit == 0)
        prime_limit = 100;

    while (1) {
        uint64_t *list;
        size_t cnt;
        build_primes_upto(prime_limit, &list, &cnt);

        if (cnt >= need) {
            free(prime_list);
            prime_list = list;
            prime_count = cnt;
            return;
        }

        free(list);
        prime_limit *= 2;
    }
}

/* Return the nth term of the sequence.  n >= 1.
   For n == 0 the function returns 0 (undefined). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;            /* safe fallback for out‑of‑range inputs */

    size_t found = 0;
    idx_t k = 1;

    while (found < n) {
        /* Need primes up to index 2*k (1‑based). */
        ensure_primes((size_t)(2 * k));

        /* difference p(2k) - p(k).  Indices are 1‑based, so subtract 1. */
        uint64_t diff64 = prime_list[2 * k - 1] - prime_list[k - 1];

        if (diff64 % 14 != 0) {
            ++k;
            continue;
        }

        uint64_t cand = diff64 / 14;
        if (is_prime((idx_t)cand)) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)cand;
        }
        ++k;
    }

    /* Control should never reach here. */
    return 0;
}
