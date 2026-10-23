/*
 * A180734  Numbers the absolute value of whose deficiency is a deficient
 *          number.
 * https://oeis.org/A180734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the integers m such that the absolute value of the
 *            deficiency ** of m is a deficient number. The function generates
 *            terms on demand
 * Range:     u8 n <= 61, u16 n <= 7727, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation utilities ---------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;
static uint64_t primes_last_checked = 1;

static void extend_primes_to_limit(uint64_t limit)
{
    if (primes == NULL) {
        primes_cap = 1024;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
        primes_last_checked = 2;
    }
    while (primes_last_checked < limit) {
        ++primes_last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > primes_last_checked)
                break;
            if (primes_last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = primes_last_checked;
        }
    }
}

static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    for (uint64_t i = 1; i * i <= n; ++i)
        ++r;
    return r;
}

/* ---------- divisor sum ---------- */
static seq_t sum_of_divisors(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t m = n;
    uint64_t limit = isqrt_uint64(m);
    extend_primes_to_limit(limit);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            seq_t power = (seq_t)1;
            seq_t sum = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;
                sum += power;
            }
            result *= sum;
        }
    }
    if (m > 1) {
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

/* ---------- sequence state ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* add value to cache */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- helper: test whether a number qualifies ---------- */
static bool qualifies(uint64_t x)
{
    seq_t sigma_x = sum_of_divisors(x);

    /* absolute deficiency */
    seq_t two_x = (seq_t)2 * (seq_t)x;
    seq_t absdef = sigma_x > two_x ? sigma_x - two_x : two_x - sigma_x;

    if (absdef == (seq_t)0)
        return false;

    /* is |deficiency| deficient? */
    seq_t sigma_absdef = sum_of_divisors((uint64_t)absdef);
    seq_t two_absdef = (seq_t)2 * absdef;
    return sigma_absdef < two_absdef;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for offset 1, but safety */

    /* generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (qualifies(next_candidate))
            cache_push((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[n - 1];
}
