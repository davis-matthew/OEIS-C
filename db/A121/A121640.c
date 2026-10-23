/*
 * A121640  a(1) = 1. a(n) = a(n-1) + (n-th integer from among those positive
 *          integers which are coprime to a(n-1)).
 * https://oeis.org/A121640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + (n-th positive integer that is coprime
 *            to a(n-1))
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* small prime sieve up to 100000 ------------------------------------ */
static uint32_t *small_primes = NULL;
static size_t small_prime_cnt  = 0;
static size_t small_prime_cap  = 0;

static void init_small_primes(void)
{
    if (small_primes != NULL)
        return;
    const size_t limit = 100000;
    bool *is_p = (bool *)malloc(limit + 1);
    if (!is_p)
        return; /* allocation failure – unlikely in test environment */
    memset(is_p, 1, limit + 1);
    is_p[0] = is_p[1] = false;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_p[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_p[j] = false;
    small_prime_cap = 128;
    small_primes = (uint32_t *)malloc(small_prime_cap * sizeof(uint32_t));
    for (size_t i = 2; i <= limit; ++i)
        if (is_p[i]) {
            if (small_prime_cnt == small_prime_cap) {
                small_prime_cap *= 2;
                small_primes = (uint32_t *)realloc(small_primes,
                                                   small_prime_cap * sizeof(uint32_t));
            }
            small_primes[small_prime_cnt++] = (uint32_t)i;
        }
    free(is_p);
}

/* ------------------------------------------------------------------ */
/* factorization: unique prime factors -------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *out, size_t *len)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < small_prime_cnt &&
         (uint64_t)small_primes[i] * small_primes[i] <= tmp; ++i) {
        uint32_t p = small_primes[i];
        if (tmp % p == 0) {
            out[cnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        out[cnt++] = tmp;
    *len = cnt;
}

/* ------------------------------------------------------------------ */
/* count of numbers <= m that are coprime to x (x > 0) ----------------*/
static uint64_t count_coprime(uint64_t m, const uint64_t *primes,
                              size_t pcnt)
{
    uint64_t res = m;
    const uint64_t subsets = (1ULL << pcnt);
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        uint64_t prod = 1;
        int bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                prod *= primes[i];
                ++bits;
                if (prod > m)
                    break;
            }
        }
        if (prod > m)
            continue;
        if (bits & 1)
            res -= m / prod;
        else
            res += m / prod;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* find the n-th integer that is coprime to X (X > 0) ----------------*/
static seq_t find_nth_coprime(seq_t X, idx_t n)
{
    uint64_t x = (uint64_t)X;
    uint64_t primes_arr[10];
    size_t pcnt = 0;
    factor_uint64(x, primes_arr, &pcnt);

    uint64_t phi = x;
    for (size_t i = 0; i < pcnt; ++i)
        phi = phi / primes_arr[i] * (primes_arr[i] - 1);

    uint64_t q   = (uint64_t)((uint64_t)n - 1) / phi;
    uint64_t r   = (uint64_t)((uint64_t)n - 1) % phi;

    uint64_t lo = 1, hi = x, target = r + 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (count_coprime(mid, primes_arr, pcnt) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t residue = lo;
    uint64_t val     = residue + q * x;
    return (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* memoised sequence a(n) -------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        exit(EXIT_FAILURE);
    }
    memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety for n < 1 */

    init_small_primes();

    ensure_cache_capacity((size_t)n + 1);

    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t nxt  = prev + find_nth_coprime(prev, (idx_t)i);
            cache[i]   = nxt;
        }
    }
    cache_len = n;
    return cache[n];
}
