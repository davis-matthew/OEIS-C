/*
 * A087765  Primes whose reversal is a multiple of 17.
 * https://oeis.org/A087765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th prime p such that reverse(p) mod 17 == 0
 * Range:     u8 n <= 5, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A087765: Primes whose reversal is a multiple of 17 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev.h"
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* Static state for caching */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t prime_count = 0;           /* number of primes in prime_list */
static idx_t prime_idx = 0;              /* next prime index to inspect */

static seq_t *seq_cache = NULL;         /* cache of sequence values */
static size_t seq_cap = 0;               /* allocated capacity of seq_cache */
static idx_t seq_len = 0;                /* number of values currently cached */

/* Helper: check whether a prime p satisfies the condition */
static inline bool valid_prime(uint64_t p)
{
    return (rev((idx_t)p) % 17) == 0;
}

/* Ensure the prime list is initialized and large enough */
static void ensure_primes(void)
{
    /* Initial limit: 10 million is enough for >10,000 terms */
    static const idx_t LIMIT = 20000000ULL;  /* 20 million */

    if (prime_list == NULL) {
        /* build_primes_upto requires uint64_t ** and size_t * */
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0, safe guard */

    if (n - 1 >= seq_len) {            /* need to generate more terms */
        ensure_primes();               /* primes ready */

        /* Ensure cache capacity */
        grow_seq(&seq_cache, &seq_cap, n);

        /* Generate terms until we have n values */
        while (seq_len < n) {
            /* Find next prime that satisfies the condition */
            while (prime_idx < prime_count && !valid_prime(prime_list[prime_idx])) {
                ++prime_idx;
            }
            /* If we ran out of primes, we should extend the sieve (unlikely) */
            if (prime_idx >= prime_count) {
                /* Increase limit and rebuild */
                idx_t new_limit = prime_count * 2;
                build_primes_upto(new_limit, &prime_list, &prime_count);
                prime_idx = 0;   /* restart scan with new list */
                continue;
            }

            /* Store the prime in the cache */
            seq_cache[seq_len] = (seq_t)prime_list[prime_idx];
            ++seq_len;
            ++prime_idx;      /* move to next prime for subsequent search */
        }
    }

    return seq_cache[n - 1];
}
