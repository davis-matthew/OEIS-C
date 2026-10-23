/*
 * A154966  Primes p such that the concatenation of p and prime(p) is
 *          composite.
 * https://oeis.org/A154966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the n‑th prime p such that the decimal concatenation of p
 *            and the p‑th prime is composite
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 14, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, build_primes_upto, concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/concat_parts.h"        /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n); */
#include "../utils/append_candidate.h"    /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

static bool initialized = false;

/* Primes list */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t    prime_cnt   = 0;    /* number of primes stored */
static idx_t     prime_limit = 5000000ULL;  /* current sieve limit */

/* Cached good primes */
static seq_t *good_primes = NULL;
static size_t good_len    = 0;
static size_t good_cap    = 0;

/* Index of next prime in prime_list to test */
static idx_t next_test_idx = 0;

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < (size_t)need) {
        prime_limit *= 2ULL;
        free(prime_list);
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* below offset – undefined behavior allowed */
        return 0;

    if (!initialized) {
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        initialized = true;
    }

    while (good_len < n) {
        /* Make sure we still have an unexplored prime. */
        if (next_test_idx >= prime_cnt) {
            prime_limit *= 2ULL;
            free(prime_list);
            build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        }

        uint64_t p = prime_list[next_test_idx++];
        /* Ensure we can access the p‑th prime (1‑based). */
        ensure_primes((idx_t)p);
        uint64_t prime_of_p = prime_list[(size_t)p - 1];

        seq_t concat_val = concat_parts((idx_t)p, (idx_t)prime_of_p);
        if (!is_prime_big(concat_val))
            append_candidate(&good_primes, &good_len, &good_cap, (seq_t)p);
    }

    return good_primes[n - 1];
}
