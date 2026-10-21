/*
 * A085061  a(0) = 1; a(n+1) is the largest positive number (of at most two)
 *          such that abs(a(n) - a(n+1)) is the smallest prime not occurring
 *          earlier as difference of successive terms and a(n) + a(n+1) is
 *          composite.
 * https://oeis.org/A085061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085061: a(0)=1; a(n+1) is the largest positive x such that
   abs(a(n)-x)=p where p is the smallest unused prime not yet used
   as a difference and a(n)+x is composite. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

static const idx_t PRIMES_LIMIT = 2000000;   /* large enough for n≈10000 */

/* ---------- prime sieve --------------------------------------------------- */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void build_primes(void)
{
    size_t lim = PRIMES_LIMIT;
    bool *is_prime = (bool*)malloc((lim + 1) * sizeof(bool));
    for (size_t i = 0; i <= lim; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= lim; j += i) is_prime[j] = false;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) ++cnt;
    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) prime_list[idx++] = i;
    prime_cnt = cnt;
    free(is_prime);
}

/* ---------- 64‑bit Miller–Rabin ------------------------------------------ */

static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t test_bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(test_bases)/sizeof(test_bases[0]); ++i) {
        uint64_t a = test_bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- primality test for arbitrary seq_t --------------------------- */
static bool is_prime_seq_t(seq_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static bool is_composite(seq_t n)
{
    if (n <= 1) return false;
    return !is_prime_seq_t(n);
}

/* ---------- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static bool *prime_used = NULL;

    if (!initialized) {
        build_primes();
        seq_cap = 32;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
        prime_used = (bool*)calloc(prime_cnt, sizeof(bool));
        initialized = true;
    }

    while (seq_len <= (size_t)n) {
        size_t prev = seq_len - 1;
        seq_t a_prev = seq[prev];
        seq_t chosen = 0;
        bool found = false;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            if (prime_used[pi]) continue;
            seq_t d = prime_list[pi];

            seq_t cand1 = a_prev + d;
            bool ok1 = false, ok2 = false;
            seq_t sum1 = a_prev + cand1;      /* 2*a_prev + d */
            if (is_composite(sum1)) ok1 = true;

            seq_t cand2 = 0;
            if (a_prev > d) {
                cand2 = a_prev - d;
                seq_t sum2 = a_prev + cand2;  /* 2*a_prev - d */
                if (is_composite(sum2)) ok2 = true;
            }

            if (ok1 || ok2) {
                if (ok1 && (!ok2 || cand1 >= cand2)) chosen = cand1;
                else chosen = cand2;
                prime_used[pi] = true;
                found = true;
                break;
            }
        }
        if (!found) chosen = a_prev;  /* safety fallback */

        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = chosen;
    }
    return seq[(size_t)n];
}
