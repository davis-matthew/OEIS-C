/*
 * A227973  Minimum composite squarefree numbers k such that p(i)-n divides
 *          k+n, for n=1, 2, 3, 4,..., where p(i) are the prime factors of k.
 * https://oeis.org/A227973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal composite square‑free integer k such that for
 *            every prime factor p of k, (p – n) divides (k + n). The
 *            implementation executes a brute‑force search for each n, using a
 *            dynamic prime list and caching of already computed terms
 * Range:     u8 n <= 1, u16 n <= 263, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Dynamic list of primes ------------------------------------- */
static uint64_t *prime_list = NULL;   /* array of primes            */
static size_t prime_cnt   = 0;        /* number of primes cached   */
static size_t prime_cap   = 0;        /* capacity of the array     */

/* Integer square root (64‑bit), using binary search.  Handles up to 2^64-1. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = (uint64_t)sqrt((long double)n);
    while ((__uint128_t)(x + 1) * (x + 1) <= n) ++x;
    while ((__uint128_t)x * x > n) --x;
    return x;
}

/* Ensure that prime_list contains all primes ≤ limit.  If not, generate
   additional primes using trial division on odd numbers. */
static void ensure_primes(uint64_t limit)
{
    if (prime_cnt == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    uint64_t candidate = prime_list[prime_cnt - 1];
    candidate += 1;
    if (candidate % 2 == 0) candidate++;

    while (candidate <= limit) {
        bool is_prime = true;
        uint64_t sq = isqrt_u64(candidate);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[prime_cnt++] = candidate;
        }
        candidate += 2;
    }
}

/* --------- Factorisation of k (k ≤ UINT64_MAX) -------------------------- */
/*   Fills primes_buf with distinct prime factors of k, if k is
    composite and square‑free.  Returns true on success, false otherwise. */
static bool factorize(uint64_t k, uint64_t primes_buf[64], size_t *cnt)
{
    *cnt = 0;
    if (k < 6) return false;          /* smallest composite square‑free is 6 */
    uint64_t tmp = k;
    uint64_t limit = isqrt_u64(tmp);
    ensure_primes(limit);

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int mult = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++mult;
                if (mult > 1) return false;   /* not square‑free */
            }
            primes_buf[(*cnt)++] = p;
            limit = isqrt_u64(tmp);
        }
    }
    if (tmp > 1) {                /* remaining factor is prime */
        primes_buf[(*cnt)++] = tmp;
    }
    if (*cnt < 2) return false;   /* not composite */
    return true;
}

/* ------------------- Search for minimal k for given n ----------------- */
static seq_t find_k_for_n(uint64_t n)
{
    for (uint64_t k = 6; ; ++k) {
        uint64_t primes_buf[64];
        size_t cnt;
        if (!factorize(k, primes_buf, &cnt)) continue;

        seq_t kn = (seq_t)k + (seq_t)n;
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes_buf[i];
            seq_t d = (p > n) ? (seq_t)(p - n) : (seq_t)(n - p);
            if (d == 0) { ok = false; break; }   /* p == n → division by zero */
            if (kn % d != 0) { ok = false; break; }
        }
        if (ok) return (seq_t)k;
    }
}

/* ----------------------------- Cache ----------------------------------- */
static seq_t *cache = NULL;      /* cache[i] = a(i), cache[0] unused      */
static idx_t cache_size = 0;     /* allocated length                         */

static void grow_cache(idx_t need)
{
    if (cache_size > need) return;
    idx_t new_size = (need + 1) * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) new_cache[i] = 0;     /* undefined for n < 1 */
        else new_cache[i] = find_k_for_n((uint64_t)i);
    }
    cache = new_cache;
    cache_size = new_size;
}

/* --------------------------- exported function -------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    grow_cache(n);
    return cache[n];
}
