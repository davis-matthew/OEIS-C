/*
 * A233439  a(n) = |{0 < k < n: prime(k)^2 + 4*prime(n-k)^2 is prime}|.
 * https://oeis.org/A233439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (0 < k < n) such that * prime(k)^2 +
 *            4*prime(n-k)^2 is prime
 * Range:     u8 n <= 1881, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (OEIS A233439) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* ---------- prime sieve up to a fixed limit ---------- */
static uint64_t *prime_list   = NULL;   /* 0‑based list of primes */
static size_t   prime_cnt    = 0;       /* number of primes available */
static bool     primes_ready = false;   /* flag whether primes are filled */

/* simple deterministic Miller–Rabin for 64‑bit integers */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }
    /* deterministic bases for < 2^64 */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

static void init_primes(void)
{
    /* The 10,000th prime is 104729; 200000 gives > 18000 primes */
    const size_t LIMIT = 200000;
    bool *mark = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!mark) return;          /* in case of OOM, bail out */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!mark[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                mark[j] = true;
        }
    }
    size_t est = LIMIT / 10;    /* <20000, enough for 200k */
    prime_list = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!prime_list) { free(mark); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!mark[i]) {
            if (idx == est) {
                est <<= 1; /* double size */
                uint64_t *tmp = (uint64_t *)realloc(prime_list, est * sizeof(uint64_t));
                if (!tmp) { free(prime_list); free(mark); return; }
                prime_list = tmp;
            }
            prime_list[idx++] = i;
        }
    }
    free(mark);
    prime_cnt    = idx;
    primes_ready = true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    if (!primes_ready) {
        init_primes();
        if (!primes_ready) return 0; /* failed to build primes, give up gracefully */
    }

    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint64_t pk   = prime_list[k - 1];
        uint64_t pnk  = prime_list[(n - k) - 1];
        uint64_t val  = pk * pk + 4ULL * pnk * pnk;
        if (is_prime_uint64(val))
            ++cnt;
    }
    return cnt;
}
