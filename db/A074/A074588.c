/*
 * A074588  Rearrangement of odd primes such that a(k) + a(k+1) + 1 is a prime
 *          for all k.
 * https://oeis.org/A074588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3644, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of A074588.
   a(1) = 3.
   For k ≥ 1, a(k+1) is the smallest unused odd prime p such that
   a(k)+p+1 is prime.  The sequence is a permutation of the odd primes. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/sieve.h"

/* Static state for the sequence */

static bool initialized = false;
static uint64_t *prime_list = NULL;            /* primes up to PRIME_LIMIT (incl. 2) */
static size_t prime_cnt = 0;                   /* number of primes in prime_list */
static bool *prime_used = NULL;                /* true if prime is already placed */
static bool *sum_prime_cache = NULL;           /* sieve for sums up to 2*PRIME_LIMIT+2 */
static size_t sum_prime_cache_size = 0;
static seq_t *seq_cache = NULL;                /* computed sequence values   */
static size_t seq_len = 0;                     /* number of terms stored     */
static size_t seq_cap = 0;                     /* capacity of seq_cache      */

#define PRIME_LIMIT 200000                    /* covers >10 000 odd primes */

/* Initialise primes and auxiliary data once */
static void init_once(void)
{
    if (initialized)
        return;

    /* Build all primes up to PRIME_LIMIT */
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);

    /* Allocate and initialise used array */
    prime_used = (bool*)calloc(prime_cnt, sizeof(bool));
    if (!prime_used) abort();

    /* Prepare prime sum cache up to 2*PRIME_LIMIT+2 */
    sum_prime_cache_size = 2 * PRIME_LIMIT + 2;
    sum_prime_cache = (bool*)calloc(sum_prime_cache_size + 1, sizeof(bool));
    if (!sum_prime_cache) abort();
    sieve(sum_prime_cache, sum_prime_cache_size);

    /* Mark prime 2 as used (it's never odd) */
    prime_used[0] = true;

    initialized = true;
}

/* Append a value to the sequence cache, expanding storage if needed */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 1;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    }
    seq_cache[seq_len++] = val;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                   /* undefined, but safe */

    init_once();                                    /* ensure primes and caches are ready */

    /* Compute terms up to required index */
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {                          /* first term is always 3 */
            seq_append(3);
            /* prime_list[1] == 3 */
            prime_used[1] = true;
        } else {
            seq_t last = seq_cache[seq_len - 1];
            seq_t next_val = 0;

            /* Search for the smallest unused odd prime that works */
            for (size_t i = 1; i < prime_cnt; ++i) {   /* start at index 1 to skip 2 */
                if (prime_used[i])
                    continue;                         /* already used */

                uint64_t p = prime_list[i];
                seq_t sum = last + (seq_t)p + 1;

                /* If sum is small enough we can test it with our sieve */
                if ((size_t)sum <= sum_prime_cache_size &&
                    sum_prime_cache[(size_t)sum]) {
                    next_val = (seq_t)p;
                    prime_used[i] = true;
                    break;
                }
            }

            if (next_val == 0) {                    /* should never happen for required range */
                abort();
            }
            seq_append(next_val);
        }
    }

    return seq_cache[n - 1];                         /* a(1) is at index 0 */
}
