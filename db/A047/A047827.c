/*
 * A047827  Becomes prime after exactly 8 iterations of f(x) = sum of prime
 *          factors of x.
 * https://oeis.org/A047827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers x such that f⁸(x) is prime, where f(x) = sum of distinct
 *            prime factors of x
 * Range:     u8 none, u16 n <= 44, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- constants and static data ---------- */
#define RAD_LIMIT 5000000ULL                /* sieve limit */

static uint32_t *rad = NULL;                /* sum of distinct prime factors */
static uint32_t *primes_list = NULL;        /* list of primes up to RAD_LIMIT */
static size_t primes_count = 0;             /* number of primes in the list */
static bool initialized = false;            /* sieve ready ? */

/* cache of sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;

/* ---------- helper functions ---------- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp > 0) {
        if (exp & 1) res = mod_mul(res, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n <= RAD_LIMIT) return rad[(size_t)n] == (uint32_t)n;

    static const uint32_t small[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint32_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* compute sum of distinct prime factors for uint64_t */
static uint64_t sum_distinct_prime_factors_uint64(uint64_t n)
{
    if (n <= RAD_LIMIT) return rad[(size_t)n];
    uint64_t tmp = n;
    uint64_t sum = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes_list[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            sum += p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp;   /* remaining prime factor */
    return sum;
}

/* --------- sieve and prime list initialization -------- */
static void init(void)
{
    if (initialized) return;
    rad = (uint32_t *)malloc((RAD_LIMIT + 1) * sizeof(uint32_t));
    if (!rad) abort();
    for (size_t i = 0; i <= RAD_LIMIT; ++i) rad[i] = 0;

    bool *isPrime = (bool *)malloc((RAD_LIMIT + 1) * sizeof(bool));
    if (!isPrime) abort();
    for (size_t i = 0; i <= RAD_LIMIT; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    for (size_t i = 2; i <= RAD_LIMIT; ++i) {
        if (isPrime[i]) {
            rad[i] = (uint32_t)i;
            if (i <= RAD_LIMIT / 2) {
                for (size_t j = i * 2; j <= RAD_LIMIT; j += i) {
                    rad[j] += (uint32_t)i;
                    isPrime[j] = false;
                }
            }
        }
    }

    /* build primes list */
    size_t cnt = 0;
    for (size_t i = 2; i <= RAD_LIMIT; ++i) if (isPrime[i]) ++cnt;
    primes_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes_list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= RAD_LIMIT; ++i) if (isPrime[i]) primes_list[idx++] = (uint32_t)i;
    primes_count = cnt;

    free(isPrime);
    initialized = true;
}

/* enlarge cache until it holds at least n terms */
static void ensure_cache_size(idx_t n)
{
    init();
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        uint64_t x = cand;
        bool ok = true;
        for (int step = 0; step < 8; ++step) {
            uint64_t xf = sum_distinct_prime_factors_uint64(x);
            if (step < 7) {
                if (is_prime_uint64(xf)) { ok = false; break; }
                x = xf;
            } else {
                if (!is_prime_uint64(xf)) ok = false;
                x = xf;
            }
        }
        if (ok) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* ---------- required public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but return 0 */
    ensure_cache_size(n);
    return cache[n - 1];
}
