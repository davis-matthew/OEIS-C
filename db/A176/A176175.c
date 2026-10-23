/*
 * A176175  Numbers k such that (2^(k-1) mod k) = number of prime divisors of
 *          k (counted with multiplicity).
 * https://oeis.org/A176175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176175: Numbers k such that (2^(k‑1) mod k) = bigomega(k) */
/* Algorithm: simple forward search with caching. */
/* Bigomega is computed by trial division using a pre‑computed prime list. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*-----------------------  prime generation  -----------------------*/
static uint32_t *prime_list = NULL;    /* array of primes */
static size_t num_primes = 0;          /* count of primes */

/* generate a list of all primes up to LIMIT (inclusive) */
static void init_primes(void)
{
    if (prime_list != NULL) return;              /* already done */

    /* a bound large enough for n up to ~10 000 (k is well below 2 000 000) */
    const size_t LIMIT = 2000000;
    uint8_t *is_comp = (uint8_t *)malloc(LIMIT + 1);
    if (!is_comp) abort();

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    /* count primes */
    num_primes = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++num_primes;

    prime_list = (uint32_t *)malloc(num_primes * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[idx++] = (uint32_t)i;

    free(is_comp);
}

/*-----------------------  big–omega  -----------------------*/
/* number of prime factors of n (counted with multiplicity) */
static uint64_t bigomega_u64(uint64_t n)
{
    if (n <= 1) return 0;

    uint64_t count = 0;
    for (size_t i = 0; i < num_primes; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        while (n % p == 0) {
            ++count;
            n /= p;
        }
    }
    if (n > 1) ++count;               /* remaining prime factor */
    return count;
}

/*-----------------------  modular power (64 bit)  -----------------------*/
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (unsigned __int128)result * base % mod;
        exp >>= 1;
        if (exp) base = (unsigned __int128)base * base % mod;
    }
    return result;
}

/*-----------------------  sequence cache  -----------------------*/
static seq_t *cache = NULL;      /* array of sequence terms */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap   = 0;    /* allocated capacity */

/* grow cache to at least need elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/*-----------------------  main function  -----------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;     /* offset is 1 */

    init_primes();                       /* ensure primes ready */

    if (cache_cap == 0) {                /* first call: allocate cache */
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = 16;
    }

    /* find required terms */
    uint64_t next_k = cache_size ? (uint64_t)cache[cache_size - 1] + 1 : 1;
    while (cache_size < (size_t)n) {
        uint64_t omega = bigomega_u64(next_k);
        uint64_t modpow = powmod_uint64(2, next_k - 1, next_k);
        if (omega == modpow) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
