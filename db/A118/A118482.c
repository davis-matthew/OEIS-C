/*
 * A118482  Partial sums of Chen primes (starting with 1).
 * https://oeis.org/A118482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, * a(n) = 1 + ∑_{k=0}^{n-1} p_k, * where p_k
 *            are the Chen primes (primes p such that p+2 is prime or a
 *            product of two primes)
 * Range:     u8 n <= 13, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses a static cache and generates primes lazily. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"
#include "../utils/is_prime_small.h"

static seq_t *cum_sums = NULL;          /* a(n) values, a(0)=1 */
static size_t cum_cap = 0;              /* allocated capacity for cum_sums */
static size_t cum_count = 0;            /* number of a(n) values stored (must be ≥1) */
static seq_t current_sum = 0;           /* current cumulative sum, aka a(cum_count-1) */

static uint64_t *prime_list = NULL;     /* array of primes up to current limit */
static size_t prime_cnt = 0;            /* number of primes in prime_list */
static size_t prime_idx = 0;            /* index of next prime to examine for Chen property */
static idx_t prime_limit = 2000000;     /* current upper bound for primes */

static bool initialized = false;

/* Test whether n ≥ 4 and has exactly two prime factors (counted with multiplicity). */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;
    int count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        while (n % p == 0) {
            ++count;
            n /= p;
            if (count > 2) return false;
        }
    }
    if (n > 1) ++count;        /* remaining prime factor */
    return count == 2;
}

/* Determine whether p is a Chen prime.  p is known to be prime. */
static bool is_chen_prime(uint64_t p)
{
    uint64_t q = p + 2;
    if (is_prime_small((seq_t)q)) return true;
    return is_semiprime(q);
}

/* Ensure the static data structures are initialized and
 * that enough Chen primes have been accumulated to cover a(n). */
static void ensure_ns(idx_t n)
{
    if (!initialized) {
        /* Allocate initial arrays */
        grow_seq((seq_t **)&cum_sums, &cum_cap, 32);
        cum_count = 1;                /* a(0) = 1 */
        cum_sums[0] = (seq_t)1;
        current_sum = (seq_t)1;

        /* Generate initial primes up to prime_limit */
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        prime_idx = 0;

        initialized = true;
    }

    /* Pull Chen primes from the primes list until we have enough sums. */
    while (cum_count <= (size_t)n) {
        if (prime_idx == prime_cnt) {
            /* Need more primes – double the limit and rebuild the list. */
            prime_limit *= 2;
            free(prime_list);
            build_primes_upto(prime_limit, &prime_list, &prime_cnt);
            /* prime_idx already points past the last processed prime,
             * which still exists in the new list.  No reset needed. */
        }

        uint64_t p = prime_list[prime_idx++];
        if (is_chen_prime(p)) {
            current_sum += (seq_t)p;
            grow_seq((seq_t **)&cum_sums, &cum_cap, cum_count + 1);
            cum_sums[cum_count++] = current_sum;
        }
    }
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    ensure_ns(n);
    return cum_sums[(size_t)n];
}
