/*
 * A162164  Primes p such that p-1 and p+1 can be written as a sum of 2
 *          distinct nonzero squares.
 * https://oeis.org/A162164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p such that p‑1 and p+1 can be written as a sum of
 *            two distinct nonzero squares. The function sieves all primes up
 *            to LIMIT, precomputes every number that is a sum of two distinct
 *            nonzero squares, and then builds the sequence on demand with
 *            memoisation
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

const uint64_t LIMIT = 20000000ULL;   /* enough for n up to ~10 000 */

/* integer square root (floor) */
static uint64_t isqrt_uint64(uint64_t n) {
    uint64_t lo = 0;
    uint64_t hi = ((uint64_t)1 << 32) - 1;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* global data */
static bool            *sumsq_dist   = NULL; /* numbers that are sum of two distinct squares */
static uint32_t        *primes       = NULL; /* list of primes up to LIMIT */
static size_t          primes_count  = 0;
static bool            primes_sieved = false;

/* sieve primes and precompute all sums of two distinct squares */
static void init_sieve_and_sumsq(void) {
    if (primes_sieved) return;

    /* ---------- sieve primes ---------- */
    bool *is_prime_arr = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime_arr) exit(1);
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime_arr[j] = false;
        }
    }
    /* collect primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime_arr[i]) ++cnt;
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime_arr[i]) primes[idx++] = (uint32_t)i;
    primes_count = cnt;
    primes_sieved = true;
    free(is_prime_arr);

    /* ---------- precompute sums of two distinct squares ---------- */
    sumsq_dist = (bool*)calloc((LIMIT + 2), sizeof(bool)); /* indexes 0 .. LIMIT+1 */
    if (!sumsq_dist) exit(1);
    uint64_t max_a = isqrt_uint64(LIMIT + 1);
    for (uint64_t a = 1; a <= max_a; ++a) {
        uint64_t a2 = a * a;
        for (uint64_t b = a + 1; b <= max_a; ++b) {
            uint64_t s = a2 + b * b;
            if (s > LIMIT + 1) break;
            sumsq_dist[s] = true;
        }
    }
}

/* ---------- memoisation for the sequence ---------- */
static seq_t *cached_vals = NULL;
static size_t cached_cap  = 0;
static size_t cached_cnt  = 0;
static size_t prime_pos   = 0;    /* next prime index to test */

static void ensure_cache_cap(size_t need) {
    if (cached_cap < need) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cached_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cached_vals = tmp;
        cached_cap  = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    init_sieve_and_sumsq();

    /* build cache up to n */
    while (cached_cnt < (size_t)n) {
        /* search next prime that satisfies the condition */
        while (prime_pos < primes_count) {
            uint32_t p = primes[prime_pos++];
            if (p < 3) continue;
            if (sumsq_dist[(size_t)(p - 1)] && sumsq_dist[(size_t)(p + 1)]) {
                ensure_cache_cap(cached_cnt + 1);
                cached_vals[cached_cnt++] = (seq_t)p;
                break;          /* found one */
            }
        }
        if (prime_pos >= primes_count && cached_cnt < (size_t)n) {
            /* LIMIT too small – should not happen for the intended range */
            return (seq_t)0;
        }
    }
    return cached_vals[(size_t)n - 1];
}
