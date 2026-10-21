/*
 * A211673  Least prime number prime(k) such that prime(n+k) - prime(k) is a
 *          power of 2.
 * https://oeis.org/A211673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime prime(k) such that prime(n+k)-prime(k) is a
 *            power of 2
 * Range:     u8 n <= 17, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (offset 1, i.e. a(1) is the first term listed). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_log2_uint64.h"

static uint64_t *prime_list   = NULL;   /* stored as uint64_t for full precision */
static size_t   prime_count   = 0;      /* number of primes in prime_list   */
static uint64_t prime_limit   = 0;      /* largest value previously sieved  */

/* Ensure that prime_list contains at least idx_needed primes (1‑based). */
static void
ensure_primes_up_to_index(idx_t idx_needed)
{
    if (prime_list && prime_count >= (size_t)idx_needed)
        return;                                 /* already enough primes */

    /* Estimate an upper bound on the value of the idx_needed-th prime.
       For n >= 6:  prime(n) < n (ln n + ln ln n).
       Using log2 as a rough substitute:  n * (floor_log2(n)+1) + 100  */
    uint64_t limit;
    if (idx_needed < 6u)
        limit = 15u;
    else
        limit = (uint64_t)idx_needed *
                (((uint64_t)floor_log2_uint64(idx_needed)) + 1u) + 100u;

    if (!prime_list || limit > prime_limit) {
        free(prime_list);                      /* free old sieve */
        build_primes_upto(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }
}

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        /* need primes up to index n+k */
        ensure_primes_up_to_index(n + k);

        uint64_t pk       = prime_list[(size_t)k - 1];
        uint64_t pk_next  = prime_list[(size_t)(n + k) - 1];
        uint64_t diff     = pk_next - pk;                /* difference in full precision */

        if (diff && (diff & (diff - 1)) == 0)            /* power‑of‑two test */
            return (seq_t)pk;
    }
}
