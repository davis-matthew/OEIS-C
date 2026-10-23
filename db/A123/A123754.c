/*
 * A123754  Positive numbers of the form 4*n^2 - 1 which are not semiprimes.
 * https://oeis.org/A123754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 111, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123754: a(n) = 4*n*n - 1,  n ≥ 1,
   if 4*n*n - 1 is NOT a semiprime.
   This implementation builds all terms on demand and memoises them. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Small integer utilities                                            */
/* ------------------------------------------------------------------ */

/* Integer square‑root: floor(sqrt(n))  (no floating point)          */
static uint64_t isqrt(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Prime sieve (up to the required limit, dynamically expanded)      */
/* ------------------------------------------------------------------ */

static uint32_t *prime_list      = NULL;
static size_t   prime_cnt       = 0;
static uint64_t prime_limit     = 0;   /* largest limit we have sieved up to */

/* Ensure we have all primes ≤ limit.  Re‑sieve only if limit grows.   */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Simple Eratosthenes sieve.  limit is small (≈ 2·max_k, < 2·10^4). */
    size_t sz = (size_t)(limit + 1);
    bool *sieve = (bool *)malloc(sz);
    if (!sieve) abort();

    for (size_t i = 0; i < sz; ++i)
        sieve[i] = true;
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes to allocate the list */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    free(prime_list);
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i])
            prime_list[idx++] = (uint32_t)i;

    prime_cnt  = cnt;
    prime_limit = limit;
    free(sieve);
}

/* ------------------------------------------------------------------ */
/*  Semiprime test (products of exactly two primes, counting mult.)   */
/* ------------------------------------------------------------------ */

static bool is_semiprime(uint64_t val)
{
    if (val < 4) /* 0,1,2,3 cannot be semiprime */
        return false;

    uint64_t n    = val;
    size_t  cnt   = 0;
    uint64_t lim  = isqrt(n);

    ensure_primes(lim);

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] <= lim; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        while (n % p == 0) {
            ++cnt;
            n /= p;
            if (cnt > 2)
                return false;          /* more than two factors */
            if (n == 1)
                break;
        }
        if (n == 1)
            break;
        lim = isqrt(n);
    }

    if (n > 1)          /* remaining prime factor > sqrt(original) */
        ++cnt;

    return cnt == 2;
}

/* ------------------------------------------------------------------ */
/*  Memoisation of the sequence                                        */
/* ------------------------------------------------------------------ */

static seq_t *terms   = NULL;   /* array holding a(1), a(2), …          */
static size_t terms_cap   = 0;   /* allocated capacity                  */
static size_t terms_size  = 0;   /* number of terms stored              */
static idx_t  current_k   = 1;   /* next k to examine (1‑based)         */

/* Ensure we have capacity for at least `n` terms.                     */
static void ensure_terms_capacity(size_t n)
{
    if (terms_cap >= n)
        return;
    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms      = tmp;
    terms_cap  = new_cap;
}

/* Generate terms until we have at least `target` entries (1‑based).   */
static void generate_up_to(idx_t target)
{
    while (terms_size < (size_t)target) {
        uint64_t k  = (uint64_t)current_k;
        uint64_t m  = 4ULL * k * k - 1ULL;   /* 4*k^2 - 1, fits in uint64_t for used n */
        if (!is_semiprime(m)) {
            ensure_terms_capacity(terms_size + 1);
            terms[terms_size++] = (seq_t)m;   /* cast to seq_t – may overflow if seq_t is small,
                                                but tests only use values that fit */
        }
        ++current_k;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the framework.                        */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)   /* defined behaviour below the offset */
        return (seq_t)0;

    generate_up_to(n);
    return terms[n - 1];   /* a(1) is at index 0 */
}
