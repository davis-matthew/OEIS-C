/*
 * A068545  Numbers n such that n*tau(n)>3*prime(n).
 * https://oeis.org/A068545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   n*tau(n) > 3*prime(n) → a(n) = the n‑th integer satisfying that
 *            inequality
 * Range:     u8 none, u16 n <= 37, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array, sieve_primes, sieve_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_spf.h"
#include "../utils/sieve_primes.h"
#include "../utils/grow_seq_array.h"

static bool initialized = false;
static uint32_t *spf = NULL;
static size_t spf_limit = 0;
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;
static idx_t cur_candidate = 1;
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void init_once(void)
{
    if (initialized)
        return;
    /* Choose a limit large enough for n up to ~10 000 and safety margin */
    const idx_t LIMIT = 5000000ULL;   /* 5 million */
    spf_limit = LIMIT;
    spf = (uint32_t*)calloc(spf_limit + 1, sizeof(uint32_t));
    if (spf == NULL) abort();
    sieve_spf(spf, spf_limit);
    sieve_primes(LIMIT, &primes, &prime_count);
    prime_limit = LIMIT;
    initialized = true;
}

static size_t divisor_count(uint64_t n)
{
    size_t d = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        d *= (size_t)(e + 1);
    }
    return d;
}

static seq_t find_nth(idx_t n)
{
    init_once();
    while (seq_len < n) {
        /* compute tau for cur_candidate */
        uint64_t cand = cur_candidate;
        size_t tau = divisor_count(cand);
        /* ensure we have enough primes to get prime(cand) */
        if ((size_t)cand > prime_count) {
            /* enlarge prime table (unlikely for n <= 10 000) */
            idx_t new_limit = prime_limit * 2;
            while (new_limit < cand * (10 / 9)) /* rough */
                new_limit *= 2;
            uint64_t *new_primes = NULL;
            size_t new_count = 0;
            sieve_primes(new_limit, &new_primes, &new_count);
            free(primes);
            primes = new_primes;
            prime_count = new_count;
            prime_limit = new_limit;
        }
        /* check inequality */
        seq_t lhs = (seq_t)cand * (seq_t)tau;
        seq_t rhs = (seq_t)3 * (seq_t)primes[cand - 1]; /* primes are 0‑based */
        if (lhs > rhs) {
            /* store candidate */
            if (seq_cap < seq_len + 1)
                grow_seq_array(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len] = (seq_t)cand;
            ++seq_len;
        }
        ++cur_candidate;
    }
    return seq_cache[n - 1];
}

seq_t a(idx_t n)
{
    return find_nth(n);
}
