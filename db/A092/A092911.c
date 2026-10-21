/*
 * A092911  Numbers all of whose divisors can be formed using their digits.
 *          Divisor digits are a subset of the digits of the number.
 * https://oeis.org/A092911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3597, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth positive integer all of whose divisors
   have a digit multiset contained in that of the integer itself.
   The function searches integers in increasing order, tests the
   property, and caches results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"   /* void digit_counts(seq_t, unsigned char[10]); */

/* Prime sieve for trial division (up to 1 000 000). */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;

static void init_prime_sieve(void)
{
    if (prime_list != NULL) return;          /* already initialized */

    const size_t LIMIT = 1000000;
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) return;                    /* unlikely to happen */

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) {
                is_comp[j] = true;
            }
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) prime_list[idx++] = (uint64_t)i;
    }
    prime_count = cnt;
    free(is_comp);
}

/* Recursively enumerate all divisors of a number (uint64_t version).
   cnt_n holds the digit counts of the original number. */
static bool check_divisors_recursive(size_t idx,
                                     uint64_t cur,
                                     const uint64_t primes[],
                                     const uint64_t exps[],
                                     size_t factor_cnt,
                                     const unsigned char cnt_n[10])
{
    if (idx == factor_cnt) {
        unsigned char cnt_d[10];
        digit_counts((seq_t)cur, cnt_d);
        for (int d = 0; d < 10; ++d) {
            if (cnt_d[d] > cnt_n[d]) return false;
        }
        return true;
    }

    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        uint64_t next = cur * mult;
        if (!check_divisors_recursive(idx + 1, next, primes, exps, factor_cnt, cnt_n))
            return false;
        if (e < exps[idx]) mult *= primes[idx];
    }
    return true;
}

/* Test whether a number satisfies the sequence property. */
static bool is_valid_number(uint64_t v)
{
    unsigned char cnt_n[10];
    digit_counts((seq_t)v, cnt_n);

    /* Digit '1' is mandatory because divisor 1 must be representable. */
    if (cnt_n[1] == 0) return false;

    /* Factorize v. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    uint64_t tmp = v;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        if (tmp % prime_list[i] == 0) {
            size_t e = 0;
            while (tmp % prime_list[i] == 0) {
                tmp /= prime_list[i];
                ++e;
            }
            primes[factor_cnt]   = prime_list[i];
            exps[factor_cnt]     = (uint64_t)e;
            ++factor_cnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        primes[factor_cnt] = tmp;
        exps[factor_cnt]   = 1;
        ++factor_cnt;
    }

    return check_divisors_recursive(0, 1, primes, exps, factor_cnt, cnt_n);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_checked = 0;

    init_prime_sieve();        /* ensure primes are available */

    while (cache_size < (size_t)n) {
        ++last_checked;
        if (is_valid_number(last_checked)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
