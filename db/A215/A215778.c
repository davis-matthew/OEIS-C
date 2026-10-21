/*
 * A215778  Natural growth of an aliquot sequence driven by a perfect number
 *          2^(p-1)*((2^p)-1), but starting at 11.
 * https://oeis.org/A215778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11, a(n+1) = 2*σ(a(n)) – a(n) where σ is sum of divisors
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- small‑prime sieve (up to 1 000 000) ---------- */
#define PRIME_LIMIT 1000000
#define MAX_PRIME_COUNT 80000
static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = 1;
    uint8_t *is_comp = (uint8_t*)calloc(PRIME_LIMIT + 1, 1);
    if (!is_comp) exit(1);
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) {
            if (prime_cnt < MAX_PRIME_COUNT)
                primes[prime_cnt++] = (uint32_t)i;
            else
                break;
        }
    free(is_comp);
}

/* ---------- sum of divisors for seq_t (collect factors) ---------- */
static seq_t sigma_of_seq(seq_t n)
{
    if (n == (seq_t)1) return (seq_t)1;
    init_primes();

    seq_t result = (seq_t)1;
    seq_t temp   = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;
        if (p2 > temp) break;
        if (temp % p != (seq_t)0) continue;

        /* factor p^e */
        seq_t term_sum = (seq_t)1;
        seq_t p_pow    = (seq_t)1;
        while (temp % p == (seq_t)0) {
            temp /= p;
            p_pow *= p;
            term_sum += p_pow;
        }
        result *= term_sum;
    }
    if (temp > (seq_t)1) {               /* remaining prime factor > sqrt(original) */
        result *= ((seq_t)1 + temp);
    }
    return result;
}

/* ---------- dynamic cache of sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

static void ensure_cache(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */

    size_t need = (size_t)n;
    ensure_cache(need);

    while (cache_cnt < need) {
        if (cache_cnt == 0) {               /* a(1) = 11 */
            cache[0] = (seq_t)11;
        } else {
            seq_t prev   = cache[cache_cnt - 1];
            seq_t sigma  = sigma_of_seq(prev);
            /* a(next) = 2*σ(prev) - prev */
            seq_t next = sigma + sigma - prev;
            cache[cache_cnt] = next;
        }
        ++cache_cnt;
    }
    return cache[n - 1];
}
