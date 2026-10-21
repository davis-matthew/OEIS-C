/*
 * A065343  Smallest k such that usigma(n+k) = usigma(k).
 * https://oeis.org/A065343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that usigma(n+k) = usigma(k)
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* usigma(m) is the unitary sum of divisors: ∏(1 + p^e) over the prime powers of m. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to 1 000 000) ---------- */
static unsigned *primes = NULL;
static size_t prime_cnt = 0;
static const unsigned PRIME_LIMIT = 1000000; /* sqrt(10^12) */

static void init_primes(void)
{
    if (primes != NULL)
        return;

    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, 1);
    if (!is_comp)
        return;  /* allocation failure – rely on runtime to handle it */

    primes = NULL;
    size_t cap = 0;

    for (unsigned p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            if (prime_cnt >= cap) {
                cap = cap ? cap * 2 : 128;
                unsigned *tmp = (unsigned*)realloc(primes, cap * sizeof(unsigned));
                if (!tmp) {
                    free(is_comp);
                    return;
                }
                primes = tmp;
            }
            primes[prime_cnt++] = p;
            if ((unsigned long long)p * p <= PRIME_LIMIT)
                for (unsigned q = p * p; q <= PRIME_LIMIT; q += p)
                    is_comp[q] = true;
        }
    }
    free(is_comp);
}

/* ---------- Cache for σ*(n) ----------------------------------- */
static seq_t *sigma_cache = NULL;
static idx_t sigma_cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n < sigma_cache_size)
        return;
    idx_t new_size = sigma_cache_size ? sigma_cache_size * 2 : 1024;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(sigma_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – nothing we can do */
        return;
    }
    if (tmp != sigma_cache) {
        memset(tmp + sigma_cache_size, 0,
               (new_size - sigma_cache_size) * sizeof(seq_t));
        sigma_cache = tmp;
    } else {
        memset(tmp + sigma_cache_size, 0,
               (new_size - sigma_cache_size) * sizeof(seq_t));
    }
    sigma_cache_size = new_size;
}

/* ---------- Unitary σ* function (using cache) ----------------- */
static seq_t sigma_star(idx_t m)
{
    if (m == 0)
        return 0;

    ensure_cache(m);
    seq_t cached = sigma_cache[m];
    if (cached != 0 || m == 1)
        return cached ? cached : (seq_t)1;

    seq_t result = 1;
    idx_t x = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        unsigned p = primes[i];
        if ((idx_t)p * (idx_t)p > x)
            break;
        if (x % p == 0) {
            idx_t e = 0;
            seq_t p_pow = 1;
            while (x % p == 0) {
                x /= p;
                ++e;
                p_pow *= (seq_t)p;
            }
            result *= (1 + p_pow);
        }
    }
    if (x > 1) {            /* remaining prime factor with exponent 1 */
        result *= (1 + (seq_t)x);
    }

    sigma_cache[m] = result;
    return result;
}

/* ---------- Sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0, return 0 as placeholder */

    init_primes();

    idx_t k = 1;
    for (;;) {
        seq_t sigma_k = sigma_star(k);
        seq_t sigma_nk = sigma_star(n + k);
        if (sigma_k == sigma_nk)
            return (seq_t)k;
        ++k;
    }
}
