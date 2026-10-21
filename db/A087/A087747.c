/*
 * A087747  Least number expressible as a sum of two distinct primes in
 *          exactly n ways.
 * https://oeis.org/A087747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number expressible as a sum of two distinct primes
 *            in exactly n ways
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------ NTT parameters and helpers ------------ */
#define MOD 998244353U
#define G   3U

static inline uint32_t modpow(uint32_t a, uint32_t e)
{
    uint32_t r = 1U;
    while (e) {
        if (e & 1U) r = (uint64_t)r * a % MOD;
        a = (uint64_t)a * a % MOD;
        e >>= 1U;
    }
    return r;
}

static void ntt(uint32_t *a, size_t n, bool invert)
{
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            uint32_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
        }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        uint32_t wlen = modpow(G, (MOD - 1) / len);
        if (invert) wlen = modpow(wlen, MOD - 2);
        for (size_t i = 0; i < n; i += len) {
            uint32_t w = 1U;
            for (size_t j = 0; j < len / 2; ++j) {
                uint32_t u = a[i + j];
                uint32_t v = (uint64_t)a[i + j + len / 2] * w % MOD;
                uint32_t sum = u + v;
                if (sum >= MOD) sum -= MOD;
                uint32_t diff = (u >= v) ? u - v : u + MOD - v;
                a[i + j] = sum;
                a[i + j + len / 2] = diff;
                w = (uint64_t)w * wlen % MOD;
            }
        }
    }
    if (invert) {
        uint32_t n_inv = modpow((uint32_t)n, MOD - 2);
        for (size_t i = 0; i < n; ++i)
            a[i] = (uint64_t)a[i] * n_inv % MOD;
    }
}

/* ------------ Cached state ------------ */
static idx_t          cur_limit = 0;      /* current sieve limit (max sum)     */
static size_t         prime_cnt = 0;     /* number of primes found            */
static idx_t         *primes    = NULL;   /* array of primes                   */
static seq_t         *min_for_count = NULL;/* minimal m for each representation count */
static size_t          max_count = 0;     /* largest count seen so far          */

/* ------------ Rebuild all data up to a new sieve limit ------------ */
static void recompute_limit(idx_t new_limit)
{
    /* free previous data */
    free(primes);
    free(min_for_count);

    /* 1. Sieve of Eratosthenes */
    size_t N = (size_t)new_limit + 1;
    bool *is_composite = (bool*)calloc(N, sizeof(bool));
    size_t est = N / 10 + 10;
    primes = (idx_t*)malloc(est * sizeof(idx_t));
    prime_cnt = 0;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt >= est) {
                est <<= 1;
                primes = (idx_t*)realloc(primes, est * sizeof(idx_t));
            }
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= (uint64_t)new_limit) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    is_composite[j] = true;
            }
        }
    }

    /* 2. Boolean array of primes (needed to detect self-pairs) */
    bool *is_prime_bool = (bool*)calloc(N, sizeof(bool));
    for (size_t i = 0; i < prime_cnt; ++i)
        is_prime_bool[primes[i]] = true;

    /* 3. Prepare convolution */
    size_t fft_size = 1;
    while (fft_size < (size_t)(new_limit + 1) * 2)
        fft_size <<= 1;
    uint32_t *a = (uint32_t*)calloc(fft_size, sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        a[primes[i]] = 1U;

    ntt(a, fft_size, false);
    for (size_t i = 0; i < fft_size; ++i)
        a[i] = (uint64_t)a[i] * a[i] % MOD;
    ntt(a, fft_size, true);

    /* 4. Compute unordered counts and track minimal values */
    max_count = 0;
    /* temporary array for counts, can be reused directly */
    size_t *counts = (size_t*)calloc(new_limit + 1, sizeof(size_t));
    for (idx_t s = 2; s <= new_limit; ++s) {
        size_t cv = a[s];
        size_t self = ( (s & 1ULL) == 0 && is_prime_bool[s >> 1] ) ? 1 : 0;
        size_t unordered = (cv - self) / 2;
        counts[s] = unordered;
        if (unordered > max_count) max_count = unordered;
    }

    min_for_count = (seq_t*)calloc(max_count + 1, sizeof(seq_t));
    for (idx_t s = 2; s <= new_limit; ++s) {
        size_t c = counts[s];
        if (c > 0 && min_for_count[c] == (seq_t)0)
            min_for_count[c] = (seq_t)s;
    }

    /* cleanup */
    free(is_composite);
    free(is_prime_bool);
    free(counts);
    free(a);

    cur_limit = new_limit;
}

/* ------------ Ensure the data covers index n ------------ */
static void ensure_up_to_n(idx_t n)
{
    if (n <= max_count) return;
    /* start with a sensible limit */
    size_t lim = cur_limit ? cur_limit : 200000ULL;
    if (lim < (size_t)n * 2) lim = (size_t)n * 2;
    /* grow until we have enough counts */
    while (n > max_count) {
        recompute_limit((idx_t)lim);
        if (n > max_count) {
            if (lim > 20000000ULL) break; /* avoid runaway */
            lim <<= 1;
        }
    }
}

/* ------------ Sequence function ------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_up_to_n(n);
    if ((idx_t)n > max_count) return 0; /* no such value yet */
    return min_for_count[n];
}
