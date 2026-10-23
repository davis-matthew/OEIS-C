/*
 * A039733  a(n)=k such that prime(k) is the prime q<prime(n) for which
 *          (prime(n) mod q) is maximal.
 * https://oeis.org/A039733
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the index k of the prime q < prime(n) that
 *            maximizes (prime(n) mod q)
 * Range:     u8 n <= 456, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses a small sieve up to 200 000, which contains more than 10 000 primes. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes_list = NULL;
static size_t primes_count = 0;
static int primes_initialized = 0;

/* Initialise the static prime table once, up to a fixed limit sufficient
   for all n <= 10 000. */
static void init_primes(void) {
    if (primes_initialized) return;
    /* 200 000 contains 17 985 primes, more than enough for n = 10 000. */
    idx_t limit = 200000;
    build_primes_upto(limit, &primes_list, &primes_count);
    primes_initialized = 1;
}

seq_t a(idx_t n) {
    /* The sequence is defined only for n >= 2. Behaviour for smaller n is
       not required. */
    if (n < 2) return (seq_t)0;

    init_primes();

    /* value of prime(n) (1‑based indexing) */
    uint64_t p = primes_list[n - 1];

    /* Find the prime q < p that maximises p % q.  In case of a tie the
       earliest (smallest) q is chosen because the loop scans increasing
       indices. */
    uint64_t max_rem = 0;
    size_t best_index = 0;           /* zero‑based index of best prime */
    for (size_t i = 0; i < (size_t)(n - 1); ++i) {
        uint64_t q = primes_list[i];
        uint64_t rem = p % q;
        if (rem > max_rem) {
            max_rem = rem;
            best_index = i;
        }
    }

    /* Return the 1‑based index k such that prime(k) = q. */
    return (seq_t)(best_index + 1);
}
