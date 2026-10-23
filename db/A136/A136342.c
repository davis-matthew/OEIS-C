/*
 * A136342  Prime numbers n such that n-x = prime, n-(x+2) = prime (twin
 *          primes #1) and n+x = prime, n+(x+2) = prime (twin primes #2).
 * https://oeis.org/A136342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime number n such that there exists x≥1 with n−x,
 *            n−(x+2), n+x, n+(x+2) all prime. The implementation scans primes
 *            in ascending order using a sieve and tests the required
 *            twin‑prime conditions
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

#define SIEVE_LIMIT 25000000UL

static bool *is_prime_arr = NULL;
static bool sieve_inited = false;

/* Build a sieve of Eratosthenes up to SIEVE_LIMIT. */
static void init_sieve(void)
{
    if (sieve_inited) return;
    size_t n = SIEVE_LIMIT;
    is_prime_arr = (bool*)malloc((n + 1) * sizeof(bool));
    if (!is_prime_arr) exit(1);
    for (size_t i = 0; i <= n; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime_arr[j] = false;
        }
    }
    sieve_inited = true;
}

/* Fast primality test using the precomputed sieve as far as possible. */
static inline bool is_prime_fast(idx_t x)
{
    if (x <= SIEVE_LIMIT)
        return is_prime_arr[(size_t)x];
    return is_prime(x);
}

/* Test whether a prime candidate 'n' satisfies the sequence condition. */
static bool satisfies_candidate(idx_t n)
{
    /* Search for an integer x ≥ 1.  The condition requires
       n−x and n−x−2 to be prime (left twin) and
       n+x and n+x+2 to be prime (right twin). */
    for (idx_t x = 1; ; ++x) {
        if (n <= x + 4) break;          /* ensures left upper ≥ 5 (smallest twin upper). */
        idx_t left = n - x;              /* potential upper member of left twin. */
        if (!is_prime_fast(left))        continue;
        if (!is_prime_fast(left - 2))    continue;
        idx_t right = n + x;             /* lower member of right twin. */
        if (!is_prime_fast(right))       continue;
        if (!is_prime_fast(right + 2))   continue;
        return true;                     /* found suitable x. */
    }
    return false;
}

/* Dynamic cache for the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void cache_add(seq_t val)
{
    if (seq_cache == NULL) {
        seq_cap = 8;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len >= seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Current candidate prime to test next. */
static idx_t current_candidate = 2;

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    assert(n >= 1 && "Sequence offset is 1.");
    init_sieve();

    while (seq_len < n) {
        /* Find the next prime candidate. */
        while (!is_prime_fast(current_candidate))
            ++current_candidate;

        /* Test the candidate. */
        if (satisfies_candidate(current_candidate))
            cache_add((seq_t)current_candidate);

        /* Advance to next integer for the next loop iteration. */
        ++current_candidate;
    }

    return seq_cache[n - 1];
}
