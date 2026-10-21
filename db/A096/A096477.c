/*
 * A096477  Subscripts for "secondary twin-primes": numbers k such that
 *          prime(prime(k)+1) - prime(prime(k)) = A073124(k) = 2.
 * https://oeis.org/A096477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that prime(prime(k)+1) - prime(prime(k)) = 2 (OEIS
 *            A096477)
 * Range:     u8 n <= 32, u16 n <= 5766, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* static state shared by the A096477 implementation */
static idx_t last_k = 0;                  /* last k processed */
static uint64_t *primes = NULL;           /* array of generated primes */
static size_t primes_len = 0;             /* number of primes in 'primes' */
static idx_t sieve_limit = 0;             /* upper limit used for the sieve */

/* result cache: 1‑based sequence a(n) */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* ensure that at least 'want' + 1 primes are available
 * (so that primes[want] is valid).  The sieve limit is doubled
 * until the required number of primes have been produced. */
static void ensure_primes_len_at_least(size_t want)
{
    if (primes_len > want)
        return;

    idx_t limit = (sieve_limit == 0) ? 100000 : sieve_limit;
    while (primes_len <= want) {
        limit *= 2;
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);
        free(primes);
        primes     = tmp;
        primes_len = cnt;
        sieve_limit = limit;
    }
}

/* ensure that the sequence cache can hold 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    grow_seq(&seq_cache, &seq_cap, need);
}

/* Return the n-th term of A096477 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but harmless */

    while (seq_len < (size_t)n) {
        /* advance to next k */
        last_k++;

        /* Ensure we have the prime at index 'last_k' */
        ensure_primes_len_at_least((size_t)last_k - 1);

        /* m = prime(k) */
        uint64_t m = primes[(size_t)last_k - 1];

        /* Ensure we can access primes[m] and primes[m-1] */
        ensure_primes_len_at_least((size_t)m);

        /* Compute the prime gap at indices m and m+1 */
        uint64_t diff = primes[(size_t)m] - primes[(size_t)m - 1];

        /* If the gap is 2, record this k */
        if (diff == 2) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)last_k;
        }
    }

    /* Return the requested term */
    return seq_cache[(size_t)n - 1];
}
